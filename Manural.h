#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Enum.h"
#include "Enemy.h"
#include "GameScene.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class Manural
{
public://�����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	Manural();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Manural();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();


	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	Scene GetNextScene() { return nextScene_; }

	int GetChangeFlag() { return changeFlag_; }
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�f�o�b�O�J����
	DebugCamera* debugCamera_ = nullptr;

	//�f�o�b�O�J�����L��
	bool isDebugCameraActive_ = false;

	Enemy* enemy_ = nullptr;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::MANUAL;

	int changeFlag_ = false;

	//�e�N�X�`���o���h��
	uint32_t textureHandle_ = 0;
	//�X�v���C�g
	Sprite* sprite_ = nullptr;
};


