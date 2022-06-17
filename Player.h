#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include <PlayerBullet.h>

#pragma once


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

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���L�����@
	Player* player_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�e
	PlayerBullet* bullet_ = nullptr;
	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;
};
