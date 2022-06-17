#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>

class PlayerBullet {
public:
	///<summary>
	///������
	/// </summary>
	/// <param name="model">���f��</param>
	
	void Initialize(Model* model, const Vector3& position);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///<summary>
	///�`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	
};