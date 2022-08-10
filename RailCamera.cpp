#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{
	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	//���[���h�g�����X�t�H�[���̏����ݒ�
	worldTransform_.translation_ = position;
	worldTransform_.rotation_ = rotation;
	//�r���[�v���W�F�N�V�����̏�����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();


}

void RailCamera::Update()
{
	//�ړ����x
	worldTransform_.translation_ += Vector3(0, 0, 0.01f);
	
	//���[���h�g�����X�t�H�[�����X�V
	worldTransform_.matWorld_ = MyMath::Identity();
	worldTransform_.matWorld_ *= MyMath::Scale(worldTransform_.scale_);
	worldTransform_.matWorld_ *= MyMath::RotX(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotY(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::RotZ(worldTransform_.rotation_);
	worldTransform_.matWorld_ *= MyMath::Trans(worldTransform_.translation_);
	
	//���[���h�s��̕��s�ړ�����
	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	Rotation_();
	
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = MyMath::Math_(forward, worldTransform_.matWorld_);
	
	//���_����O���ɓK���ȋ����i��
	forward += viewProjection_.eye;
	viewProjection_.target = forward;

	//���[���h���
	Vector3 up(0, 1, 0);

	//���[���J�����̉�]
	viewProjection_.up = MyMath::Math_(up, worldTransform_.matWorld_);

	//�r���[�v���W�F�N�V����
	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	//�f�o�b�O�p�\��
	debugText_->SetPos(20, 100);
	debugText_->Printf(
		"RailCamera Pos:(%f,%f,%f)", viewProjection_.eye.x,
		viewProjection_.eye.y,
		viewProjection_.eye.z);
}


void RailCamera::Rotation_(){

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= 0.01f;
	}
	else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += 0.01f;
	}
}