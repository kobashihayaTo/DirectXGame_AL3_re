#include "Enemy.h"
#include "MyMath.h"
#include <cassert>


void Enemy::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Red.png");

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = { 5,0,40 };

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
	}
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Trans(worldTransform_.translation_);
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
	Vector3 move = { 0,0,0 };
	const float speed = 0.003f;
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
	Vector3 velocity(0, 0, kBulletSpeed);
	//�v���C���[�̃��[���h���W���擾
	Vector3 playerPos = player_->GetWorldPosition();
	//�G�L�����̃��[���h���W���擾
	Vector3 enemyPos = GetWorldPosition();
	//�G�L����->���L�����̍����x�N�g�������߂�
	Vector3 velocity = Math_::Vector3Sub(playerPos, enemyPos);
	//�x�N�g���̐��K��
	velocity = Math_::normalize(velocity);
	//�x�N�g���̒������A�����ɍ��킹��
	velocity *= kBulletSpeed;





	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = Math_(velocity, worldTransform_.matWorld_);
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
	if (fileTimer <= 0)
	{
		//�e�̔���
		Fire();
		//���˃^�C�}�[��������
		fileTimer = kFireInterval;
	}
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += ApproachVelocity_;
	//����̈ʒu�ɒB�����痣�E
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}
//���E�t�F�[�Y
void Enemy::LeaveVelocity() {
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += LeaveVelocity_;
}