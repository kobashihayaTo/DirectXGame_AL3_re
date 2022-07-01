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

//自機クラスの前方宣言
class Player;

enum class Phase{
	Approach,//接近する
	Leave,   //離脱する
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
	/// <summary>
	/// ワールド座標を所得
	/// </summary>
	Vector3 GetWorldPosition();

	//発射間隔
	static const int kFireInterval = 30;


private:
	///<summary>
	///接近
	/// </summary>
	void ApproachVelocity();

	///<summary>
	///離脱
	/// </summary>
	void LeaveVelocity();
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//デバック用表示
	DebugText* debugText_ = nullptr;
	Input* input_ = nullptr;
	//フェーズ
	Phase phase_ = Phase::Approach;

	//速度
	Vector3 ApproachVelocity_ = { 0,0,-0.1f };
	Vector3 LeaveVelocity_ = { 0,0,-0.1f };

	//弾
	std::list<std::unique_ptr<EnemyBullet>>bullets_;

	//発射タイマー
	int32_t fileTimer = 0;

	//自キャラ
	Player* player_ = nullptr;
};
