#include "Enemy.h"
#include "MyMath.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Red.png");

	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = { 0,2,0 };

}

void Enemy::Update() {
	Translation();
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Trans(worldTransform_.translation_);
	//行列の再計算
	worldTransform_.TransferMatrix();
}

void Enemy::Translation()
{
	Vector3 move = { 0,0,0 };
	const float speed = 0.3f;
	move.z -= speed;

	worldTransform_.translation_ += move;
}

void Enemy::Draw(const ViewProjection& viewProjection){
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}