#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
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


};
