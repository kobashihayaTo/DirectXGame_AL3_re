#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "EnemyBullet.h"

#include "EnemyBullet.h"
#include "MyMath.h"
#include "Player.h"

//���@�N���X�̑O���錾
class Player;

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
	///�e����
	///<summary>
	void Fire();

	///<summary>
	///�ڋ߃t�F�[�Y������
	/// </summary>
	void ApproachPhaseInt();
	/// <summary>
	/// �v���C���[�̃A�h���X���Z�b�g
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition();

	//���ˊԊu
	static const int kFireInterval = 30;


private:
	///<summary>
	///�ڋ�
	/// </summary>
	void ApproachVelocity();

	///<summary>
	///���E
	/// </summary>
	void LeaveVelocity();
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
	Vector3 ApproachVelocity_ = { 0,0,-0.1f };
	Vector3 LeaveVelocity_ = { 0,0,-0.1f };

	//�e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//���˃^�C�}�[
	int32_t fileTimer = 0;

	//���L����
	Player* player_ = nullptr;
};
