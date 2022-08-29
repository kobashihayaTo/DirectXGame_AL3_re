#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include "EnemyBullet.h"


//自機クラスの前方宣言
class Player;

enum class Phase{
	Approach,//接近する
	 //次の行動にする
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
	///初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///更新
	/// </summary>
	void Update();

	void Translation();
	///<summary>
	///描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	///<summary>
	///弾発射
	///<summary>
	void Fire();

	///<summary>
	///接近フェーズ初期化
	/// </summary>
	void ApproachPhaseInt();

	/// <summary>
	/// プレイヤーのアドレスをセット
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

	//衝突が起きたら呼び出されるコールバック変数
	void OnCollision();

	bool IsDead()const { return isDead_; }

	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition();
	
	//発射間隔
	static const int kFireInterval = 30;

	//弾リストを追加
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	float GetRadius();

	int GetHp();

	void Reset();

private:

	///<summary>
	///接近
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
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	uint32_t hptextHandle = 0;
	//デバック用表示
	DebugText* debugText_ = nullptr;
	Input* input_ = nullptr;
	//フェーズ
	Phase phase_ = Phase::Approach;

	//速度
	Vector3 ApproachVelocity_ =		{ 0,    0,-0.1f };
	Vector3 LeaveVelocity_ =		{ -0.5f,0,   0 };
	Vector3 YVelocity_ =			{ 0,   0.1f,  0 };
	Vector3 XVelocity_ =			{ 0.5f,0,0 };
	Vector3 YDown_ =				{0,-0.3f,0};

	Vector3 Fastcoordinate = { 5,0,40 };

	Vector3 move = { 0,0,0 };
	float speed = 0.0f;
	float Adjspeed = 0.3f;
	//弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//発射タイマー
	int32_t fileTimer = 0;

	//自キャラ
	Player* player_ = nullptr;

	//半径
	const float radius_ = 1.0f;

	//デスフラグ
	bool isDead_ = false;

	//HPを表示
	int Hp_ = 20;

	//スプライト
	Sprite* sprite_ = nullptr;
};
