#include "GameScene.h"
#include "Enemy.h"
#include "Player.h"

#include "TextureManager.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>
#include <cassert>

#define PI 3.14

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
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

#pragma region キャラ 
	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_,textureHandle_);

	//敵キャラの生成
	enemy_ = new Enemy();
	//敵キャラの初期化
	enemy_->Initialize(model_, enemyHandle_);

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
#pragma endregion
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(20.0f);
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
}

void GameScene::Update()
{
	//自キャラの更新
	player_->Update();
	enemy_->Update();
	////視点の移動速さ
	//const float KEyeSpeed = 0.01f;
	//const float kCharacterSpeed = 0.2f;
	////クリップ距離変更処理
	////上キーで視野角が広がる
	//if (input_->PushKey(DIK_UP)) {
	//	/*viewProjection_.fovAngleY += KEyeSpeed;*/
	//	//ニアクリップ距離を増
	//	viewProjection_.nearZ++;
	//}
	////下キーで視野角が狭まる
	//else if (input_->PushKey(DIK_DOWN)) {
	//	/*viewProjection_.fovAngleY -= KEyeSpeed;*/
	//	//ニアクリップ距離を減
	//	viewProjection_.nearZ--;
	//}
	//viewProjection_.fovAngleY = Clamp(0.0f, PI, viewProjection_.fovAngleY);
	////行列の再計算
	//viewProjection_.UpdateMatrix();
	////デバック用表示
	//debugText_->SetPos(50, 110);
	//debugText_->Printf("fovAngle(Degree);%f", (180 / PI) * viewProjection_.fovAngleY);
	//debugText_->SetPos(50, 130);
	//debugText_->Printf("nearZ:%f",
	//	viewProjection_.nearZ);

	////キャラクター移動処理
	//{
	//	//キャラクターの移動ベクトル
	//	Vector3 move = { 0,0,0 };

	//	if (input_->PushKey(DIK_RIGHT)) {
	//		move = { kCharacterSpeed,0,0 };
	//	}
	//	else if (input_->PushKey(DIK_LEFT)) {
	//		move = { -kCharacterSpeed,0,0 };
	//	}
	//}
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
	//自キャラの描画
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	// 3Dモデル描画 


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


