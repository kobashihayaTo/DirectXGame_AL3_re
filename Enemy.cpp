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

	switch (phase_)
	{
	case Phase::Approach:
		ApproachVelocity();
		break;
	case Phase::Leave:
		LeaveVelocity();
		break;
	}


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

void Enemy::ApproachVelocity() {
	//移動(ベクトルを加算)
	worldTransform_.translation_ += ApproachVelocity_;
	//特定の位置に達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveVelocity(){
	//移動(ベクトルを加算)
	worldTransform_.translation_ += LeaveVelocity_;
}