#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DebugText.h"
#include "Input.h"

#include "MyMath.h"

#include <cassert>

///<summary>
///レールカメラ
///</summary>
class RailCamera
{
public:
	///<summary>
	///初期化
	///</summary>
	void Initialize(const Vector3& position,const Vector3& rotation);

	///<summary>
	///更新
	///</summary>
	void Update();

	///<summary>
	///ビュープロジェクションを取得
	///</summary>
	const ViewProjection& GetViewProjection() { return viewProjection_; }

	//ワールド行列を取得
	WorldTransform* GetWorldMatrix() { return &worldTransform_; }

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation_();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Input* input_ = nullptr;
};

