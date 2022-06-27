#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
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
	///接近
	/// </summary>
	void ApproachVelocity();

	///<summary>
	///離脱
	/// </summary>
	void LeaveVelocity();
private:
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
	Vector3 ApproachVelocity_ = { 0,0,-1.0f };
	Vector3 LeaveVelocity_ = { -0.1f,0.1f,-0.1f };
};
