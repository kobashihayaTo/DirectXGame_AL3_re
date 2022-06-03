#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include <random>

#define PI 3.14


//単位行列
Matrix4 Identity() {
	Matrix4 matIdentity;
	matIdentity.m[0][0] = 1;
	matIdentity.m[1][1] = 1;
	matIdentity.m[2][2] = 1;
	matIdentity.m[3][3] = 1;
	return matIdentity;
}
//スケーリング行列を宣言
Matrix4 Scale(Vector3 scale_) {
	//スケーリングの設定
	Matrix4 matScale;
	matScale.m[0][0] = scale_.x;
	matScale.m[1][1] = scale_.y;
	matScale.m[2][2] = scale_.z;
	matScale.m[3][3] = 1;
	return matScale;
}
Matrix4 RotZ(Vector3 rotation_) {
	//Z軸回転行列を宣言
	Matrix4 matRotZ;
	matRotZ = Identity();
	matRotZ.m[0][0] = cos(rotation_.z);
	matRotZ.m[0][1] = sin(rotation_.z);
	matRotZ.m[1][0] = -sin(rotation_.z);
	matRotZ.m[1][1] = cos(rotation_.z);
	return matRotZ;
}
Matrix4 RotX(Vector3 rotation_) {
	//X軸回転行列を宣言
	Matrix4 matRotX;
	matRotX = Identity();
	matRotX.m[1][1] = cos(rotation_.x);
	matRotX.m[1][2] = sin(rotation_.x);
	matRotX.m[2][1] = -sin(rotation_.x);
	matRotX.m[2][2] = cos(rotation_.x);
	return matRotX;
}
Matrix4 RotY(Vector3 rotation_) {
	//Y軸回転行列を宣言
	Matrix4 matRotY;
	matRotY = Identity();
	matRotY.m[0][0] = cos(rotation_.y);
	matRotY.m[0][2] = -sin(rotation_.y);
	matRotY.m[2][0] = sin(rotation_.y);
	matRotY.m[2][2] = cos(rotation_.y);
	return matRotY;
}
//平行移動行列を宣言
Matrix4 Trans(Vector3 translation_) {
	Matrix4 matTrans;
	matTrans = Identity();
	matTrans.m[3][0] = translation_.x;
	matTrans.m[3][1] = translation_.y;
	matTrans.m[3][2] = translation_.z;
	return matTrans;
}



GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
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
	// 3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransforms_[0].Initialize();

	////親0番
	//worldTransforms_[0].Initialize();
	////子1番
	//worldTransforms_[1].Initialize();
	//worldTransforms_[1].translation_ = { 0,4.5f,0 };
	//worldTransforms_[1].parent_ = &worldTransforms_[0];
	
	//キャラクターの大元
	worldTransforms_[PartId::kRoot].Initialize();
	//脊髄
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,2.5f,0 };
#pragma region 上半身
	//胸
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_= &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };
	//頭
	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0,3.5f,0 };
	//左腕
	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -2.5f,0,0 };
	//右腕
	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 2.5f,0,0 };
#pragma endregion
#pragma region 下半身
	//尻
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kHip].translation_ = { 0,0,0 };
	//左足
	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -2.5f,-2.5f,0 };
	//右足
	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 2.5f,-2.5f,0 };
#pragma endregion
	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = Angle(30.0f);
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

	//デバックカメラの生成
	debugCamera_ = new DebugCamera(920, 120);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示の参照するビュープロジェクションを指定する
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ライン描画が参照するビュープロジェクションを指定する
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update()
{
	//視点の移動速さ
	const float KEyeSpeed = 0.01f;
	const float kCharacterSpeed = 0.2f;
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

	//キャラクター移動処理
	{
		//キャラクターの移動ベクトル
		Vector3 move = { 0,0,0 };

		

		if (input_->PushKey(DIK_RIGHT)) {
			move = { kCharacterSpeed,0,0 };
		}
		else if (input_->PushKey(DIK_LEFT)) {
			move = { -kCharacterSpeed,0,0 };
		}

		worldTransforms_[PartId::kRoot].translation_.x += move.x;
		worldTransforms_[PartId::kRoot].translation_.y += move.y;
		worldTransforms_[PartId::kRoot].translation_.z += move.z;

		//worldTransforms_[0].matWorld_ = Identity();
		//worldTransforms_[0].matWorld_ *= Scale(worldTransforms_[0].scale_);
		//worldTransforms_[0].matWorld_ *= RotX(worldTransforms_[0].rotation_);
		//worldTransforms_[0].matWorld_ *= RotY(worldTransforms_[0].rotation_);
		//worldTransforms_[0].matWorld_ *= RotZ(worldTransforms_[0].rotation_);
		//worldTransforms_[0].matWorld_ *= Trans(worldTransforms_[0].translation_);

		/*worldTransforms_[0].TransferMatrix();*/
		//デバック
		debugText_->SetPos(50, 150);
		debugText_->Printf("Root(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x,
			worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);

		for (int i = 0; i < kNumPartId; i++)
		{
			worldTransforms_[i].matWorld_ = Identity();
			worldTransforms_[i].matWorld_ *= Scale(worldTransforms_[i].scale_);
			worldTransforms_[i].matWorld_ *= RotX(worldTransforms_[i].rotation_);
			worldTransforms_[i].matWorld_ *= RotY(worldTransforms_[i].rotation_);
			worldTransforms_[i].matWorld_ *= RotZ(worldTransforms_[i].rotation_);
			worldTransforms_[i].matWorld_ *= Trans(worldTransforms_[i].translation_);
		
			if (worldTransforms_[i].parent_ != nullptr) {
				worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
			}
			worldTransforms_[i].TransferMatrix();
		}
	}
	//子の更新(子は子で初期化)
	//{
	//	worldTransforms_[1].matWorld_ = Identity();
	//	worldTransforms_[1].matWorld_ *= Scale(worldTransforms_[1].scale_);
	//	worldTransforms_[1].matWorld_ *= RotX(worldTransforms_[1].rotation_);
	//	worldTransforms_[1].matWorld_ *= RotY(worldTransforms_[1].rotation_);
	//	worldTransforms_[1].matWorld_ *= RotZ(worldTransforms_[1].rotation_);
	//	worldTransforms_[1].matWorld_ *= Trans(worldTransforms_[1].translation_);
	//	//２つ目
	//	worldTransforms_[1].matWorld_ *= worldTransforms_[1].parent_->matWorld_;

	//	worldTransforms_[1].TransferMatrix();
	//}

	//上半身回転処理
	{
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U))
		{
			worldTransforms_[PartId::kChest].rotation_.y -= kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_I)) 
		{
			worldTransforms_[PartId::kChest].rotation_.y += kCharacterSpeed;
		}
	}
	//下半身回転処理
	{
		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J))
		{
			worldTransforms_[PartId::kHip].rotation_.y -= kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_K))
		{
			worldTransforms_[PartId::kHip].rotation_.y += kCharacterSpeed;
		}
	}
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
	//大元から順に描画していく
	for (int i = 0; i< kNumPartId; i++)
	{
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
		if (i == 0 || i == 1)
		{
			continue;
		}
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


