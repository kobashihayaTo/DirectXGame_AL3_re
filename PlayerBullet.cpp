#include "PlayerBullet.h"
#include "MyMath.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	worldTransform_.Initialize();
	//�����Ŏ󂯎�����������W�����Z�b�g
	worldTransform_.translation_ = position;
}
void PlayerBullet::Update()
{
	worldTransform_.matWorld_ = Identity();
	worldTransform_.matWorld_ *= Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= Trans(worldTransform_.translation_);
	//�s��̍Čv�Z
	worldTransform_.TransferMatrix();
}
void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	//���f���̕`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}


