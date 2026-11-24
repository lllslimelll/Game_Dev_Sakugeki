#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Engine/Scene/SceneBase.h"
#include "../../Engine/Scene/AsyncLoadingScreen.h"
#include "../Common/Fader.h"
#include "../Resource/ResourceManager.h"
#include "../Audio/Audio.h"
#include "../Scene/ScreenManager.h"
#include "../Core/Application.h"
#include "../Debug/DebugSystem.h"
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
	// フェード
	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// 画面遷移中判定
	isSceneChanging_ = true;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D用の設定
	Init3D();

	// 初期シーンの設定
	nextScene_ = CreateFirstScene();
	DoChangeScene();
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
		if (loading_)
		{
			// ロード待ち
			UpdateLoading();
		}
		else
		{
			// フェード状態の切替処理
			Fade();
		}
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
	SetDrawScreen(mainScreen);

	// 画面を初期化
	ClearDrawScreen();

	if (loading_)
	{
		// ロード中は新シーンを描画しない
		loading_->Draw();
	}
	else
	{
		// Effekseerにより再生中のエフェクトを更新
		UpdateEffekseer3D();

		// スタック内のシーンを描画
		for (auto& scene : scenes_)
		{
			scene->Draw();
		}

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		// デバッグ用の 3D 表示
		Application::GetInstance().GetDebugSystem().DrawWorld();
	}
	
	// 暗転・明転
	fader_->Draw();

	// 背面スクリーンにメインスクリーンを描画
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();
	DrawGraph(0, 0, mainScreen, true);
}

void SceneManager::Destroy(void)
{
	// 読み込み途中のハンドルを残したまま解放しないために待機
	WaitHandleASyncLoadAll();

	// シーンリストの破棄
	scenes_.clear();
	nextScene_.reset();
	loading_.reset();

	// インスタンスのメモリ解放
	delete instance_;
}

// シーン遷移
void SceneManager::ChangeScene(std::unique_ptr<SceneBase> nextScene)
{
	// フェードが終わってから切り替えるので、
	// 遷移先シーンをメンバ変数に保持
	nextScene_ = std::move(nextScene);

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;
}

// シーン遷移実行
void SceneManager::DoChangeScene()
{
	// リソースの解放
	ResourceManager::GetInstance().Release();

	// シーンリストの破棄
	scenes_.clear();

	// 新シーンをスタックに追加
	scenes_.push_back(std::move(nextScene_));

	// ロード待ち画面を作り、新シーンの Load を非同期で発行する
	loading_ = std::make_unique<AsyncLoadingScreen>();
	loading_->Load();
	loading_->StartLoad(*scenes_.back());

	fader_->SetFade(Fader::STATE::NONE);

	// 旧シーン破棄などにかかった時間を、ローディングの経過時間に含めない
	ResetDeltaTime();
}

// シーンの追加（オーバーレイ）
void SceneManager::PushOverlay(std::unique_ptr<SceneBase> overlay)
{ 
	// 旧シーンの上に新シーンを追加する
	scenes_.push_back(std::move(overlay));

	// オーバーレイシーンはリソースが少ないため、非同期ロードはしない
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

void SceneManager::ResetTime(void)
{
	preTime_ = std::chrono::system_clock::now();
}

float SceneManager::GetDeltaTime(void) const
{
	return deltaTime_;
}

SceneManager::SceneManager(void)
{
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
}

SceneManager::~SceneManager(void)
{
}

void SceneManager::UpdateLoading(void)
{
	loading_->Update(deltaTime_);

	// ロードと演出が終わるまで待つ
	if (!loading_->IsFinished()) { return; }

	// ロード待ち画面を破棄
	loading_.reset();

	// 末尾シーンの初期化
	scenes_.back()->Init();

	// デルタタイムリセット
	ResetDeltaTime();

	// 暗転から明転へ
	fader_->SetFade(Fader::STATE::FADE_IN);
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
			DoChangeScene();
		}
		break;
	}
}

