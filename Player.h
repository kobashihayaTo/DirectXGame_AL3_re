#pragma once
#include "model.h"
#include "WorldTransform.h"
#include "input.h"
#include "DebugText.h"
#include "ViewProjection.h"
#include "Audio.h"

#include <PlayerBullet.h>
#include <MyMath.h>

#include <cassert>
#include <memory>
#include <list>

#pragma once

class RailCamera;

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

	//衝突が起きたら呼び出されるコールバック変数
	void OnCollision();	

	bool IsDead()const { return isDead_; }

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//弾リストを追加
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// 半径を所得
	/// </summary>
	float GetRadius();

	/// <summary>
	/// 初期化まとめ
	/// </summary>
	void Reset();

	//Vector3 SetRailCamera(RailCamera* camera) { camera_ = camera; }

	void SetParent(WorldTransform* worldTransform) {
		worldTransform_.parent_ = worldTransform;
	}

private:

	Audio* audio_ = nullptr;
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	////サウンドハンドル
	//uint32_t soundDataHandle_ = 0u;
	
	//自キャラ　
	Player* player_ = nullptr;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>>bullets_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	//半径
	const float radius_ = 1.0f;

	RailCamera* camera_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	//HPを表示
	int PlayerHp = 20;

	Vector3 Fastcoordinate = { 0,0,0 };
};
