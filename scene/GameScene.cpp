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
	delete player_;
	delete enemy_;
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

	////カメラ垂直方向視野角を設定
	//viewProjection_.fovAngleY = Angle(20.0f);

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);
#pragma region アスペクト,ニア,ファー
	////アスペクト比を設定
	//viewProjection_.aspectRatio = 1.0f;
	////ニアクリップ距離を設定
	//viewProjection_.nearZ = 52.0f;
	////ファークリップ距離を設定
	//viewProjection_.farZ = 53.0f;
#pragma endregion
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

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyHandle_);
	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	//天球の生成
	skydome_= new Skydome();

	skydome_->Initialize();
#pragma endregion
}

void GameScene::Update()
{
	if (input_->TriggerKey(DIK_B)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}
	
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
	enemy_->Update();
	skydome_->Update();
	CheckAllCollision();
	
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

	enemy_->Draw(viewProjection_);

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
	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
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
	//自キャラの座標
	posA = enemy_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
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

#pragma region 自弾と敵弾の当たり判定
	//自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bulletA : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& bulletB : enemyBullets) {

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