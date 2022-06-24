#include "PlayerBullet.h"
#include "MyMath.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position,const Vector3& velocity)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	//引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}
void PlayerBullet::Update()
{
	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Trans(worldTransform_.translation_);
	//行列の再計算
	worldTransform_.TransferMatrix();

	//時間経過でデス
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


