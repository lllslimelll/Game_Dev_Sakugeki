#include <DxLib.h>
#include "../../Engine/Core/Application.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../UI/Menu.h"
#include "PauseScene.h"

PauseScene::PauseScene()
	:
	SceneBase()
{
}

PauseScene::~PauseScene()
{
	SetMouseDispFlag(false);
}

void PauseScene::Init()
{
	SetMouseDispFlag(true);

	menu_ = std::make_unique<Menu>(Menu::Layout{ {760, 400}, 400, 60, 16 });
	menu_->AddItem(L"ゲーム再開", [this] { sceMng_.PopOverlay(); });
	menu_->AddItem(L"設定", [this] {  });
	menu_->AddItem(L"タイトルへ戻る", [this] { sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE); });
	menu_->AddItem(L"ゲーム終了", [] { Application::GetInstance().RequestExit(); });
}

void PauseScene::Update()
{
	// ポーズ / キャンセルで閉じる
	if (input_.IsTriggered(Input::Command::PAUSE) ||
		input_.IsTriggered(Input::Command::CANCEL))
	{
		sceMng_.PopOverlay();
		return;
	}

	menu_->Update();
}

void PauseScene::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	menu_->Draw();
}

void PauseScene::UpdateMenu(void)
{
}

void PauseScene::DrawMenu(void)
{
}
