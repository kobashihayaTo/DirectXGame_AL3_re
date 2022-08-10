#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"

#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "Enum.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public:
	//パーツID
	enum PartId {
		kRoot, //大元
		kSpine,//脊髄
		kChest,//胸
		kHead,//頭
		kArmL, //左腕
		kArmR, //右腕
		kHip,  //尻
		kLegL, //左足
		kLegR, //右足

		kNumPartId
	};



public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	float Angle(float angle);
	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollision();

	Scene GetNextScene() { return nextScene_;}

	int GetIsEnd() { return isEnd_; }

	void SceneChange();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//テクスチャバンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyHandle_ = 0;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	// 3Dモデル
	Model* model_ = nullptr;

	Player* player_ = nullptr;

	Enemy* enemy_ = nullptr;
	//-----ここに敵リストを追加----
	


	//-----------------------------
	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	std::unique_ptr<RailCamera> railCamera_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	Scene nextScene_ = Scene::TITLE;

	int isEnd_ = false;
};