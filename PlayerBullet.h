#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>

class PlayerBullet {
	//����<frm>
	static const int32_t kLineTime = 60 * 5;
public:
	///<summary>
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///<summary>
	///�`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }
	//�Փ˂��N������Ăяo�����R�[���o�b�N�ϐ�
	void OnCollision();

	Vector3 GetWorldPosition();
	
	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;

	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLineTime;
	//�f�X�t���O
	bool isDead_ = false;

	//���a
	const float radius_ = 1.0f;
};