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

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class EndScene
{
public://�����o�֐�

	/// <summary>
	/// �R���X�g�N���^
	/// </summary>
	EndScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~EndScene();

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

	int GetEndFlag() { return endFlag_; }
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//�f�o�b�O�J����
	DebugCamera* debugCamera_ = nullptr;

	//�f�o�b�O�J�����L��
	bool isDebugCameraActive_ = false;

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::END;

	int endFlag_ = false;
};

