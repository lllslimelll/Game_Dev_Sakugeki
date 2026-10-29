#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Input/Input.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"
#include "../Scene/ScreenManager.h"
#include "../Audio/Audio.h"
#include "../Common/FpsController.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{
	// アプリケーションの初期設定
	SetWindowText(L"咲撃");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, COLOR_BIT_DEPTH);
	ChangeWindowMode(true);

	// FPS制御初期化
	fpsController_ = std::make_unique<FpsController>(FRAME_RATE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// 乱数のシード値を設定する
	DATEDATA date;

	// 現在時刻を取得する
	GetDateTime(&date);

	// 乱数の初期値を設定する
	// 設定する数値によって、ランダムの出方が変わる
	SRand(date.Year + date.Mon + date.Day + date.Hour + date.Min + date.Sec);

	SetUseDirectInputFlag(true);

	Input::CreateInstance();
	ResourceManager::CreateInstance();
	ScreenManager::GetInstance().Init();
	Audio::CreateInstance();
	SceneManager::CreateInstance();
}

void Application::Run(void)
{
	Input& Input = Input::GetInstance();
	SceneManager& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && !isRequestedExit_)
	{

		Input.Update();
		sceneManager.Update();

		sceneManager.Draw();

#ifdef _DEBUG

		// 平均FPS描画
		fpsController_->Draw();

#endif // _DEBUG

		ScreenFlip();

		// 理想FPS経過待ち
		fpsController_->Wait();
	}
}

void Application::Destroy(void)
{
	Input::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	Audio::GetInstance().Destroy();
	ScreenManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	// インスタンスのメモリ解放
	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

bool Application::RequestExit()
{
	isRequestedExit_ = true;
	return isRequestedExit_;
}

Application::Application(void)
	:
	fpsController_(nullptr),
	isInitFail_(false),
	isReleaseFail_(false),
	isRequestedExit_(false)
{
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}
