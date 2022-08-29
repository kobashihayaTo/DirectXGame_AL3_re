#include "GameScene.h"
#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>
#include <cassert>

#include "Player.h"

#define PI 3.14

GameScene::GameScene() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("Player.png");

	enemyhptextHandle = TextureManager::Load("EnemyHp.png");
	playerhptextHandle= TextureManager::Load("Hp.png");
	/*audio_->Initialize("Resources/shot.wav");*/

	//keytextHandle = TextureManager::Load("操作キー.png");
	// 3Dモデルの生成
	model_ = Model::Create();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1200, 720);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	////軸方向表示を有効にする
	//AxisIndicator::GetInstance()->SetVisible(true);
	////軸方向表示の参照するビュープロジェクションを指定する
	//AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

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
#pragma endregion

	//レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	//レールカメラの生成
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f));

	player_->SetParent(railCamera_->GetWorldMatrix());

	//天球の生成
	skydome_ = new Skydome();
	skydome_->Initialize();

	//スプライトの生成
	sprite_ = Sprite::Create(enemyhptextHandle, { 0,0 });
	sprite_->SetPosition({ 0,0 });

	//keysprite_ = Sprite::Create(keytextHandle, { 0,0 });
	//keysprite_->SetPosition({ 0,100 });
}

GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;

	delete sprite_;
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

#pragma region キャラ 

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵キャラの初期化
	enemy_->Initialize(model_, enemyHandle_);


#pragma endregion

	//レールカメラの生成
	railCamera_->Initialize(Vector3(0.0f, 0.0f, -50.0f), Vector3(0.0f, 0.0f, 0.0f));

	//シーン切り替え
	nextScene_ = Scene::GAME;
}

void GameScene::Update()
{

	//自キャラの更新
	player_->Update();
	enemy_->Update();
	skydome_->Update();
	CheckAllCollision();
	railCamera_->Update();


	DamageCount();

	//ゲームシーンに適応させる
	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	//ここでシーンチェンジ
	SceneChange();
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
	sprite_->Draw();

	//keysprite_->Draw();
	
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


	//自弾と敵キャラ全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		//自弾の座標
		posB = bullet->GetWorldPosition();
		//敵キャラの座標
		posA = enemy_->GetWorldPosition();

		//AとBの距離を求める
		Vector3 len = MyMath::Vector3sub(posA, posB);
		float distance = MyMath::length(len);

		//自弾と敵キャラの半径
		float radius = bullet->GetRadius() + enemy_->GetRadius();

		//自弾と敵キャラの交差判定
		if (distance <= radius) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			//自弾の衝突時コールバックを呼び出す
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

void GameScene::SceneChange() {
	isEnd_ = false;
	if (player_->IsDead() || enemy_->IsDead()) {

   		nextScene_ = Scene::END;
		isEnd_ = true;
	}
}

void GameScene::DamageCount()
{
	sprite_->SetSize({ EnemyDamageCounter,32 });

	int isDamage_ = enemy_->GetHp();

	if (isDamage_ == 19)
	{
		 EnemyDamageCounter = 570.0f;
	}
	if (isDamage_ == 18)
	{
		 EnemyDamageCounter = 540.0f;
	}
	if (isDamage_ == 17)
	{
		 EnemyDamageCounter = 510.0f;
	}
	if (isDamage_ == 16)
	{
		 EnemyDamageCounter = 480.0f;
	}
	if (isDamage_ == 15)
	{
		 EnemyDamageCounter = 450.0f;
	}
	if (isDamage_ == 14)
	{
		 EnemyDamageCounter = 420.0f;
	}
	if (isDamage_ == 13)
	{
		 EnemyDamageCounter = 390.0f;
	}
	if (isDamage_ == 12)
	{
		 EnemyDamageCounter = 360.0f;
	}
	if (isDamage_ == 11)
	{
		 EnemyDamageCounter = 330.0f;
	}
	if (isDamage_ == 10)
	{
		 EnemyDamageCounter = 300.0f;
	}
	if (isDamage_ == 9)
	{
		 EnemyDamageCounter = 270.0f;
	}
	if (isDamage_ == 8)
	{
		 EnemyDamageCounter = 240.0f;
	}
	if (isDamage_ == 7)
	{
		 EnemyDamageCounter = 210.0f;
	}
	if (isDamage_ == 6)
	{
		 EnemyDamageCounter = 180.0f;
	}
	if (isDamage_ == 5)
	{
		 EnemyDamageCounter = 150.0f;
	}
	if (isDamage_ == 4)
	{
		 EnemyDamageCounter = 120.0f;
	}
	if (isDamage_ == 3)
	{
		 EnemyDamageCounter = 90.0f;
	}
	if (isDamage_ == 2)
	{
		 EnemyDamageCounter = 60.0f;
	}
	if (isDamage_ == 1)
	{
		 EnemyDamageCounter = 30.0f;
	}
}

void GameScene::Reset()
{
	EnemyDamageCounter = 600.0f;
	enemy_->Reset();
	player_->Reset();
	railCamera_->Reset();
}
