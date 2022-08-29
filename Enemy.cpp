#include "Enemy.h"
#include <cassert>
#include "Player.h"
#include "MyMath.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Enemy.png");
	
	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { Fastcoordinate };

	worldTransform_.Initialize();

	//�ڋ߃t�F�[�Y������
	ApproachPhaseInt();

}

void Enemy::Update() {
	Translation();

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
		{
			return bullet->IsDead();
		});

	switch (phase_)
	{
	case Phase::Approach:
		ApproachVelocity();
		break;

	case Phase::Leave:
		LeaveVelocity();
		break;

	case Phase::Y:
		YVelocity();
		break;

	case Phase::X:
		XVelocity();
		break;
	case Phase::YDown:
		YdownVelocity();
		break;
	}
	worldTransform_.matWorld_ = MyMath::Identity();
	worldTransform_.matWorld_ *= MyMath::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MyMath::RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::Trans(worldTransform_.translation_);
	//�s��̍Čv�Z
	worldTransform_.TransferMatrix();

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Update();
	}

}

void Enemy::Translation()
{
	move = { 0,0,0 };
	speed = 0.03f;
	move.z -= speed;

	worldTransform_.translation_ += move;

}

void Enemy::Draw(const ViewProjection& viewProjection) {

	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�̕`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_)
	{
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {


	//�G�̍��W�R�s�[
	Vector3 position = worldTransform_.translation_;
	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//�v���C���[�̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�G�L����->���L�����̍����x�N�g�������߂�
	Vector3 velocity = MyMath::Vector3sub(playerPos, enemyPos);
	//�x�N�g���̐��K��
	velocity = MyMath::normalize(velocity);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;

	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = MyMath::Math_(velocity, worldTransform_.matWorld_);
	//�e�𐶐����A������
	std::unique_ptr<EnemyBullet>newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, position, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachPhaseInt()
{
	//���˃^�C�}�[��������
	fileTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() {
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
//�ڋ߃t�F�[�Y
void Enemy::ApproachVelocity() {
	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer--;
	//�w�莞�ԂɒB����
	if (fileTimer <= -1)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += ApproachVelocity_;
	//����̈ʒu�ɒB������
	if (worldTransform_.translation_.z < -1.0f) {
		phase_ = Phase::Leave;
	}
}

//�ڍs�t�F�[�Y
void Enemy::LeaveVelocity() {
	move = { 0,0,0 };
	speed = 0.03f;
	move.z += speed;
	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer--;
	//�w�莞�ԂɒB����
	if (fileTimer <= -1)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += move;
	worldTransform_.translation_ += LeaveVelocity_;
	//����̈ʒu�ɒB������
	if (worldTransform_.translation_.x < -2.0f) {
		phase_ = Phase::Y;
	}
}

void Enemy::YVelocity()
{
	move = { 0,0,0 };
	speed = 0.001f;
	move.z -= speed;
	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer--;
	//�w�莞�ԂɒB����
	if (fileTimer <= -1)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += move;
	worldTransform_.translation_ += YVelocity_;
	//����̈ʒu�ɒB������
	if (worldTransform_.translation_.y >3.0f) {
		phase_ = Phase::X;
	}
}

void Enemy::XVelocity()
{
	move = { 0,0,0 };
	speed = 0.001f;
	move.z -= speed;
	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer--;
	//�w�莞�ԂɒB����
	if (fileTimer <= -1)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += move;
	worldTransform_.translation_ += XVelocity_;
	//����̈ʒu�ɒB������
	if (worldTransform_.translation_.x > 3.0f) {
		phase_ = Phase::YDown;
	}
}

void Enemy::YdownVelocity()
{
	move = { 0,0,0 };
	speed = 0.001f;
	move.z -= speed;
	//���˃^�C�}�[�J�E���g�_�E��
	fileTimer--;
	//�w�莞�ԂɒB����
	if (fileTimer <= -1)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += move;
	worldTransform_.translation_ += YDown_;
	//����̈ʒu�ɒB������
	if (worldTransform_.translation_.y < -4.0f) {
		phase_ = Phase::Leave;
	}
}

 void Enemy::OnCollision() {

	 Hp_--;

	if (Hp_ <= 0)
	{
		//�f�X�t���O�̗�����
 		isDead_ = true;
	}
}

 void Enemy::Reset() {
	 fileTimer = 1;
	 phase_ = Phase::Approach;
	 worldTransform_.translation_ = { Fastcoordinate };
	 Hp_ = 20;
	 isDead_ = false;
 }

float Enemy::GetRadius() { return radius_; }

int Enemy::GetHp() { return Hp_; }