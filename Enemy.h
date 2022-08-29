#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "EnemyBullet.h"


//���@�N���X�̑O���錾
class Player;

enum class Phase{
	Approach,//�ڋ߂���
	 //���̍s���ɂ���
	Leave,  
	Y,
	X,
	YDown
};

enum class Break {
	normal,
	Damage
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

	//�Փ˂��N������Ăяo�����R�[���o�b�N�ϐ�
	void OnCollision();

	bool IsDead()const { return isDead_; }

	/// <summary>
	/// ���[���h���W������
	/// </summary>
	Vector3 GetWorldPosition();
	
	//���ˊԊu
	static const int kFireInterval = 30;

	//�e���X�g��ǉ�
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float GetRadius();

	int GetHp();

	void Reset();

private:

	///<summary>
	///�ڋ�
	/// </summary>
	void ApproachVelocity();

	///<summary>
	///
	/// </summary>
	void LeaveVelocity();

	///<summary>
	///
	/// </summary>
	void YVelocity();

	///<summary>
	///
	/// </summary>
	void XVelocity();

	///<summary>
	///
	/// </summary>
	void YdownVelocity();


private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;
	//���f��
	Model* model_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	uint32_t hptextHandle = 0;
	//�f�o�b�N�p�\��
	DebugText* debugText_ = nullptr;
	Input* input_ = nullptr;
	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	//���x
	Vector3 ApproachVelocity_ =		{ 0,    0,-0.1f };
	Vector3 LeaveVelocity_ =		{ -0.5f,0,   0 };
	Vector3 YVelocity_ =			{ 0,   0.1f,  0 };
	Vector3 XVelocity_ =			{ 0.5f,0,0 };
	Vector3 YDown_ =				{0,-0.3f,0};

	Vector3 Fastcoordinate = { 5,0,40 };

	Vector3 move = { 0,0,0 };
	float speed = 0.0f;
	float Adjspeed = 0.3f;
	//�e
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//���˃^�C�}�[
	int32_t fileTimer = 0;

	//���L����
	Player* player_ = nullptr;

	//���a
	const float radius_ = 1.0f;

	//�f�X�t���O
	bool isDead_ = false;

	//HP��\��
	int Hp_ = 20;

	//�X�v���C�g
	Sprite* sprite_ = nullptr;
};
