#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>

class PlayerBullet {
	//寿命<frm>
	static const int32_t kLineTime = 60 * 5;
public:
	///<summary>
	///初期化
	/// </summary>
	/// <param name="model">モデル</param>
	
	void Initialize(Model* model, const Vector3& position,const Vector3& velocity);

	///<summary>
	///更新
	/// </summary>
	void Update();

	///<summary>
	///描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead()const { return isDead_; }
	//衝突が起きたら呼び出されるコールバック変数
	void OnCollision();

	Vector3 GetWorldPosition();
	
	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//速度
	Vector3 velocity_;

	//デスタイマー
	int32_t deathTimer_ = kLineTime;
	//デスフラグ
	bool isDead_ = false;

	//半径
	const float radius_ = 1.0f;
};