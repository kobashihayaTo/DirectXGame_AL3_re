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
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::ApproachVelocity() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += ApproachVelocity_;
	//����̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::LeaveVelocity(){
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += LeaveVelocity_;
}