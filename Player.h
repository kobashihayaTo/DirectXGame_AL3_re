#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "Audio.h"

#include <PlayerBullet.h>
#include <MyMath.h>

#include <cassert>
#include <memory>
#include <list>

#pragma once

class RailCamera;

///<summary>
///���L����
///</summary>
class Player {

public:
	///<summary>
	///������
	///</summary>
	///<param name="model">���f��</param>
	///<param name="textureHandle">�e�N�X�`���n���h��</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///��]����
	///</summary>
	void Rotation();

	///<summary>
	///�`��
	///</summary>
	void Attack();

	///<summary>
	///�`��
	///</summary>
	void Draw(ViewProjection& viewProjection_);

	//�Փ˂��N������Ăяo�����R�[���o�b�N�ϐ�
	void OnCollision();	

	bool IsDead()const { return isDead_; }

	//���[���h���W���擾
	Vector3 GetWorldPosition();

	//�e���X�g��ǉ�
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// ���a������
	/// </summary>
	float GetRadius();

	/// <summary>
	/// �������܂Ƃ�
	/// </summary>
	void Reset();

	//Vector3 SetRailCamera(RailCamera* camera) { camera_ = camera; }

	void SetParent(WorldTransform* worldTransform) {
		worldTransform_.parent_ = worldTransform;
	}

private:

	Audio* audio_ = nullptr;
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	////�T�E���h�n���h��
	//uint32_t soundDataHandle_ = 0u;
	
	//���L�����@
	Player* player_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�e
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	//���a
	const float radius_ = 1.0f;

	RailCamera* camera_ = nullptr;

	//�f�X�t���O
	bool isDead_ = false;

	//HP��\��
	int PlayerHp = 20;

	Vector3 Fastcoordinate = { 0,0,0 };
};
