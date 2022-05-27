﻿#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>

#define PI 3.14

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

float GameScene::Angle(float angle)
{
	return angle * PI / 180;
}

//クランプ関数
//float GameScene::MinNum(float num, float num2)
//{
//	if (num < num2){
//		return num;
//	}
//	return num2;
//}
//float GameScene::MaxNum(float num, float num2)
//{
//	if (num > num2) {
//		return num;
//	}
//	return num2;
//}

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
	// 3Dモデルの生成
	model_ = Model::Create();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセデス・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の指定(回転と座標)
	std::uniform_real_distribution<float> rotation(0, 2 * PI);//回転
	std::uniform_real_distribution<float> coordinate(-10, +10);//座標
	//乱数エンジンを渡し、指定範囲っからランダムな数値を得る


	for (WorldTransform& worldTransform_ : worldTransforms_)
	{
		//ワールドトランスフォームの初期化
		worldTransform_.Initialize();

		//カメラ垂直方向視野角を設定
		viewProjection_.fovAngleY = Angle(30.0f);
		////アスペクト比を設定
		//viewProjection_.aspectRatio = 1.0f;
		//ニアクリップ距離を設定
		viewProjection_.nearZ = 52.0f;
		//ファークリップ距離を設定
		viewProjection_.farZ = 53.0f;
		//ビュープロジェクションの初期化
		viewProjection_.Initialize();

		//デバックカメラの生成
		debugCamera_ = new DebugCamera(1280, 720);

		//軸方向表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
		//軸方向表示の参照するビュープロジェクションを指定する
		AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

		//ライン描画が参照するビュープロジェクションを指定する
		PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

		//X,Y,Z方向にスケーリングを設定
		worldTransform_.scale_ = { 1,1,1 };

		//x,y,z軸周りの回転角を設定
		worldTransform_.rotation_ = { rotation(engine),rotation(engine),rotation(engine) };

		//x,y,z軸周りの平行移動を設定
		worldTransform_.translation_ = { coordinate(engine), coordinate(engine), coordinate(engine) };

		//スケーリング行列を宣言
		Matrix4 matScale;

		/*
		  Sx,0,0,0
		  0,Sy,0,0
		  0,0,Sz,0
		  0,0,0,1
		  */
		  //スケーリングの設定
		matScale.m[0][0] = worldTransform_.scale_.x;
		matScale.m[1][1] = worldTransform_.scale_.y;
		matScale.m[2][2] = worldTransform_.scale_.z;
		matScale.m[3][3] = 1;
		//単位行列
		Matrix4 matIdentity;
		/*
			1,0,0,0
			0,1,0,0
			0,0,1,0
			0,0,0,1
		*/
		matIdentity.m[0][0] = 1;
		matIdentity.m[1][1] = 1;
		matIdentity.m[2][2] = 1;
		matIdentity.m[3][3] = 1;

		//合成用宣言
		Matrix4 matRot = matIdentity;
		//各種用回転行列を宣言
		Matrix4 matRotZ, matRotX, matRotY;

#pragma 

		/*
			cos,sin,0,0
			-sin,cos,0,0
			0,0,1,0
			0,0,0,1
		*/
		//Z軸回転行列を宣言
		matRotZ = matIdentity;
		matRotZ.m[0][0] = cos(worldTransform_.rotation_.z);
		matRotZ.m[0][1] = sin(worldTransform_.rotation_.z);
		matRotZ.m[1][0] = -sin(worldTransform_.rotation_.z);
		matRotZ.m[1][1] = cos(worldTransform_.rotation_.z);

		/*
			1,0,0,0
			0,cos,sin,0
			0,-sin,cos,0
			0,0,0,1
		*/

		//X軸回転行列を宣言
		matRotX = matIdentity;
		matRotX.m[1][1] = cos(worldTransform_.rotation_.x);
		matRotX.m[1][2] = sin(worldTransform_.rotation_.x);
		matRotX.m[2][1] = -sin(worldTransform_.rotation_.x);
		matRotX.m[2][2] = cos(worldTransform_.rotation_.x);

		/*
			cos,0,-sin,0
			0,1,0,0
			sin,0,cos,0
			0,0,0,1
		*/

		//Y軸回転行列を宣言
		matRotY = matIdentity;
		matRotY.m[0][0] = cos(worldTransform_.rotation_.y);
		matRotY.m[0][2] = -sin(worldTransform_.rotation_.y);
		matRotY.m[2][0] = sin(worldTransform_.rotation_.y);
		matRotY.m[2][2] = cos(worldTransform_.rotation_.y);
		/*worldTransform_.matWorld_ *= matScale;*/  //スケーリング

		/*worldTransform_.matWorld_ *= matRotZ*/;//Z軸周りの回転
		//worldTransform_.matWorld_ *= matRotX;//X軸周りの回転
		/*worldTransform_.matWorld_ *= matRotY*/;//Y軸周りの回転


		//各種の回転行列の合成
		matRot *= matRotZ;
		matRot *= matRotX;
		matRot *= matRotY;
		worldTransform_.matWorld_ = matIdentity;
		/*worldTransform_.matWorld_ *= matRot;*/

		//平行移動行列を宣言
		Matrix4 matTrans = MathUtility::Matrix4Identity();

		//移動量を行列に設定する
#pragma region Z
		matTrans.m[0][2] = 0;
		matTrans.m[1][2] = 0;
		matTrans.m[3][2] = worldTransform_.translation_.z;
#pragma endregion
#pragma region X
		matTrans.m[1][0] = 0;
		matTrans.m[2][0] = 0;
		matTrans.m[3][0] = worldTransform_.translation_.x;
#pragma endregion
#pragma region Y
		matTrans.m[0][1] = 0;
		matTrans.m[2][1] = 0;
		matTrans.m[3][1] = worldTransform_.translation_.y;
#pragma endregion
#pragma region W
		matTrans.m[0][3] = 0;
		matTrans.m[1][3] = 0;
		matTrans.m[2][3] = 0;
#pragma endregion
		//worldTransform_.matWorld_ = matIdentity;


		//行列の合成
		worldTransform_.matWorld_ *= matScale;//スケーリング
		worldTransform_.matWorld_ *= matRot;//回転行列
		worldTransform_.matWorld_ *= matTrans;//平行移動
		//行列の転送
		worldTransform_.TransferMatrix();
	}
}

void GameScene::Update()
{
	//視点の移動速さ
	const float KEyeSpeed = 0.01f;

	//クリップ距離変更処理
	//上キーで視野角が広がる
	if (input_->PushKey(DIK_UP)) {
		/*viewProjection_.fovAngleY += KEyeSpeed;*/
		//ニアクリップ距離を増
		viewProjection_.nearZ++;
	}
	//下キーで視野角が狭まる
	else if (input_->PushKey(DIK_DOWN)) {
		/*viewProjection_.fovAngleY -= KEyeSpeed;*/
		//ニアクリップ距離を減
		viewProjection_.nearZ--;
	}
	viewProjection_.fovAngleY = Clamp(0.0f, PI, viewProjection_.fovAngleY);
	//行列の再計算
	viewProjection_.UpdateMatrix();
	//デバック用表示
	debugText_->SetPos(50, 110);
	debugText_->Printf("fovAngle(Degree);%f", (180 / PI) * viewProjection_.fovAngleY);
	debugText_->SetPos(50, 130);
	debugText_->Printf("nearZ:%f", 
		viewProjection_.nearZ);
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
	for (WorldTransform& worldTransform_ : worldTransforms_) {
		model_->Draw(worldTransform_, viewProjection_, textureHandle_);

	}

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 1, 0, 1 });

	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0, 0, 0 }, { 10, 0, 0 }, { 1, 0, 0, 1 });
	PrimitiveDrawer::GetInstance()->DrawLine3d({ 0, 0, 0 }, { 0, 0, 10 }, { 0, 0, 1, 1 });

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


