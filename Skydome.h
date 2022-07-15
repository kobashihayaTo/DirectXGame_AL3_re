#pragma once
#include "MyMath.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
///<summary>
///天球
///<summary>
class Skydome {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewproject_);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//3Dモデル
	Model* model_ = nullptr;
};

