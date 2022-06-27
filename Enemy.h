#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
enum class Phase{
	Approach,//�ڋ߂���
	Leave,   //���E����
};
class Enemy
{
public:
	///<summary>
	///������
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	void Translation();
	///<summary>
	///�`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///�ڋ�
	/// </summary>
	void ApproachVelocity();

	///<summary>
	///���E
	/// </summary>
	void LeaveVelocity();
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//�f�o�b�N�p�\��
	DebugText* debugText_ = nullptr;
	Input* input_ = nullptr;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//���x
	Vector3 ApproachVelocity_ = { 0,0,-1.0f };
	Vector3 LeaveVelocity_ = { -0.1f,0.1f,-0.1f };
};
