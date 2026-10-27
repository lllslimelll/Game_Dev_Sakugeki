#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Game/Scenes/SceneBase.h"
#include "../../Game/Scenes/TitleScene.h"
#include "../../Game/Scenes/GameScene.h"
#include "../../Game/Scenes/PauseScene.h"
#include "../../Game/Scenes/OptionScene.h"
#include "../../Game/Scenes/DeadScene.h"
#include "../Common/Fader.h"
#include "../../Game/Actor/Camera.h"
#include "../Resource/ResourceManager.h"
#include "../Audio/Audio.h"
#include "../Scene/ScreenManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	// フェード
	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->Init();

	// 画面遷移中判定
	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(
		BACKGROUND_COLOR_R, 
		BACKGROUND_COLOR_G, 
		BACKGROUND_COLOR_B);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	//SetUseLighting(true);
}

void SceneManager::Update(void)
{
	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	// フェードの更新
	fader_->Update();

	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		Fade();
	}
	else
	{
		// スタック末尾のみ更新
		scenes_.back()->Update();
	}
}

void SceneManager::Draw(void)
{
	// メインスクリーンの取得
	int mainScreen = ScreenManager::GetInstance().GetMainScreen();
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen);

	// 画面を初期化
	ClearDrawScreen();

	// カメラ設定
	camera_->SetBeforeDraw();

	// Effekseerにより再生中のエフェクトを更新
	UpdateEffekseer3D();

	// スタック内のシーンを描画
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
	
	// 暗転・明転
	fader_->Draw();

	// 背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, true);
}

void SceneManager::Destroy(void)
{
	// シーンリストの破棄
	scenes_.clear();

	// インスタンスのメモリ解放
	delete instance_;
}

// シーン遷移
void SceneManager::ChangeScene(SCENE_ID nextId)
{
	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;
	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

// シーン遷移実行
void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンを変更する
	sceneId_ = sceneId;

	// シーンリストの破棄
	scenes_.clear();

	// シーンの生成と追加
	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scenes_.push_back(std::make_unique<TitleScene>());
		break;
	case SCENE_ID::GAME:
		scenes_.push_back(std::make_unique<GameScene>());
		break;
	}

	// 一旦ロードここに置く
	scenes_.back()->Load();
	// シーン末尾の初期化
	scenes_.back()->Init();

	// デルタタイムリセット
	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;
}

// オーバーレイの追加（オーバーレイとして）
void SceneManager::PushOverlay(SCENE_ID sceneId)
{ 
	// シーンの変更
	sceneId_ = sceneId;
	
	// シーンの生成
	switch (sceneId_)
	{
	case SCENE_ID::PAUSE:
		scenes_.push_back(std::make_unique<PauseScene>());
		break;
	case SCENE_ID::OPTION:
		scenes_.push_back(std::make_unique<OptionScene>());
		break;
	case SCENE_ID::DEAD:
		scenes_.push_back(std::make_unique<DeadScene>());
		break;
	}

	// 一旦ロードここに置く
	scenes_.back()->Load();
	// シーン末尾の初期化
	scenes_.back()->Init();
}

// オーバーレイの削除
void SceneManager::PopOverlay()
{
	if (scenes_.size() > 1)
	{
		scenes_.pop_back();
	}
}

Camera& SceneManager::GetCamera(void)
{
	return *camera_;
}


SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

SceneManager::SceneManager(void)
{
	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::Fade(void)
{
	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}

