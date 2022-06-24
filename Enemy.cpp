#include "Enemy.h"
#include "MyMath.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Red.png");

	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
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
	//�s��̍Čv�Z
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
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}