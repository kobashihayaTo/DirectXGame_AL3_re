#pragma once
#include "Model.h"
#include "Vector3.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
class  EnemyBullet
{
	
public:
	///<summary>
   ///������
   /// </summary>
   /// <param name="model">���f��</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	///<summary>
	///�X�V
	/// </summary>
	void Update();

	///<summary>
	///�`��
	/// </summary>
	void Draw(const ViewProjection& viewProjection);


	bool IsDead()const { return isDead_; }
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	//���x
	Vector3 velocity_;
	//����
	static const int32_t kLineTime = 60 * 5;
	//�f�X�^�C�}�[
	int32_t deathTimer_ = kLineTime;
	//�f�X�t���O
	bool isDead_ = false;

};

