#include "EnemyBullet.h"
#include "MyMath.h"


void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	model_ = model;
	velocity_ = velocity;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("EnemyBullet.png");
	//�����Ŏ󂯎�����������W�����Z�b�g
	worldTransform_.translation_ = position;

	worldTransform_.Initialize();
}

void EnemyBullet::Update()
{
	//���W���ړ�������
	worldTransform_.translation_-= velocity_;

	worldTransform_.matWorld_ = MyMath::Identity();
	worldTransform_.matWorld_ *= MyMath::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MyMath::RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::Trans(worldTransform_.translation_);
	//�s��̍Čv�Z
	worldTransform_.TransferMatrix();

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0)
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void EnemyBullet::OnCollision()
{
	//�f�X�t���O�̗�����
	isDead_ = true;
}
Vector3 EnemyBullet::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
float EnemyBullet::GetRadius() { return radius_; }