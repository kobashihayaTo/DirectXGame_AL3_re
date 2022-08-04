#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>
#include <cassert>

#include "Player.h"

#define PI 3.14

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	//自キャラの解放
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}



float Clamp(float min, float max, float num) {
	if (min > num) {
		return min;
	}
	else if (max < num) {
		return max;
	}
	return num;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	enemyHandle_ = TextureManager::Load("Red.png");

	// 3Dモデルの生成
	model_ = Model::Create();


	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

		//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示の参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

#pragma region キャラ 
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);


#pragma endregion

	//レールカメラの生成
	railCamera_ =std::make_unique<RailCamera>();
	//レールカメラの生成
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f));

	player_->SetParent(railCamera_->GetWorldMatrix());

	//天球の生成
	skydome_ = new Skydome();
	skydome_->Initialize();
}

void GameScene::Update()
{
	
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	else {
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();
	}

	//自キャラの更新
	player_->Update();
	for(const std::unique_ptr<Enemy>&enemy:enemys_){
		enemy->Update();
	}
	skydome_->Update();
	CheckAllCollision();
	railCamera_->Update();

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet :bullets_)
	{
		bullet->Update();
	}

	//ゲームシーンに適応させる
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画 
	// 自キャラの描画
	player_->Draw(viewProjection_);

	for (const std::unique_ptr<Enemy>& enemy : enemys_){
		enemy->Draw(viewProjection_);
	}

	//弾の描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollision()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();


#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		//AとBの距離を求める
		Vector3 len = MyMath::Vector3sub(posA, posB);
		float distance = MyMath::length(len);

		//自キャラと敵弾の半径
		float radius = player_->GetRadius() + bullet->GetRadius();

		//自キャラと敵弾の交差判定
		if (distance <= radius) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<Enemy>& enemy : enemys_) {
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
		{
			//自弾の座標
			posB = bullet->GetWorldPosition();
			//敵のキャラの座標
			posA = enemy->GetWorldPosition();
			//AとBの距離を求める
			Vector3 len = MyMath::Vector3sub(posA, posB);
			float distance = MyMath::length(len);

			//敵キャラと敵弾の半径
			float radius = player_->GetRadius() + bullet->GetRadius();

			//自キャラと敵弾の交差判定
			if (distance <= radius) {
				//自キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();
				//敵弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : bullets_) {

			//自弾の座標
			posB = bulletA->GetWorldPosition();
			//敵弾の座標
			posA = bulletB->GetWorldPosition();

			Vector3 len = MyMath::Vector3sub(posA, posB);
			//座標AとBの距離を求める
			float distance = MyMath::length(len);

			//自弾と敵弾の半径
			float radius = bulletB->GetRadius() + bulletA->GetRadius();

			//自弾と敵弾の交差判定
			if (distance <= radius) {
				bulletB->OnCollision();
				bulletA->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemy(Vector3 pos)
{
	// スマートポインタ (使いやすいのがユニークポインタ)
	std::unique_ptr<Enemy>enemy = std::make_unique<Enemy>();

	enemy->Initialize(model_, textureHandle_);

	//リストに登録する
	enemys_.push_back(std::move(enemy));
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//リストに登録する
	bullets_.push_back(std::move(enemyBullet));
}
