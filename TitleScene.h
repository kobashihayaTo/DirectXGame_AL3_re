#pragma once
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Enum.h"
#include "Enemy.h"
#include "GameScene.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene
{
public://メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	TitleScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	void Draw();

	Scene GetNextScene() { return nextScene_; }

	int GetChangeFlag() { return changeFlag_; }
private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	Enemy* enemy_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::TITLE;

	int changeFlag_ = false;

	//テクスチャバンドル
	uint32_t textureHandle_ = 0;
	//スプライト
	Sprite* sprite_ = nullptr;
};


