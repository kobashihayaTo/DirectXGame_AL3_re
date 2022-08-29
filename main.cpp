#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"

#include "TitleScene.h"
#include "EndScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	GameScene* gameScene = nullptr;

	TitleScene* titleScene = nullptr;
	EndScene* endScene = nullptr;

	//----追加------

	Scene scene = Scene::TITLE;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow("LE2C_13_コバシ_ハヤト");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize();

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion
	//タイトルの初期化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();
	
	//エンドの初期化
	endScene = new EndScene();
	endScene->Initialize();
	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// 入力関連の毎フレーム処理
		input->Update();

		//---追加---
		switch (scene)
		{
		case Scene::TITLE :
			titleScene->Update();
			if (titleScene->GetChangeFlag()) {
				gameScene->Reset();
				scene = titleScene->GetNextScene();
			}
			break;
		case Scene::GAME:
			// ゲームシーンの毎フレーム処理
			gameScene->Update();
			if (gameScene->GetIsEnd()) {
 				scene = gameScene->GetNextScene();
			}
			break;
		case Scene::END:
			endScene->Update();
			if (endScene->GetEndFlag()) {
				scene = endScene->GetNextScene();
			}
			break;
		}

		//--------


		// 軸表示の更新
		axisIndicator->Update();

		// 描画開始
		dxCommon->PreDraw();

		//---追加---
		switch (scene)
		{
		case Scene::TITLE:
			titleScene->Draw();
			break;
		case Scene::GAME:
			// ゲームシーンの描画
			gameScene->Draw();
			break;
		case Scene::END:
			endScene->Draw();
			break;
		}

		//--------

		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	SafeDelete(gameScene);
	audio->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}