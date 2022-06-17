#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include <cassert>
#include <PlayerBullet.h>

#pragma once


///<summary>
///自キャラ
///</summary>
class Player {

public:
	///<summary>
	///初期化
	///</summary>
	///<param name="model">モデル</param>
	///<param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

	///<summary>
	///更新
	///</summary>
	void Update();
	///<summary>
	///回転処理
	///</summary>
	void Rotation();
	///<summary>
	///描画
	///</summary>
	void Attack();
	///<summary>
	///描画
	///</summary>
	void Draw(ViewProjection& viewProjection_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	//自キャラ　
	Player* player_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//弾
	PlayerBullet* bullet_ = nullptr;
	//ビュープロジェクション
	ViewProjection viewProjection_;
};
