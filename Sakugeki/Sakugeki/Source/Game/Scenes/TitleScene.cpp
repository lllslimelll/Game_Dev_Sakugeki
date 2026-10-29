#include <DxLib.h>
#include "../../Engine/Core/Application.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Input/Input.h"
#include "../Actor/Camera.h"
#include "../UI/Menu.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	SceneBase()
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Load(void)
{
}

void TitleScene::Init(void)
{
	SetMouseDispFlag(true);

	constexpr int ITEM_W = 400;
	float x = (Application::SCREEN_SIZE_X - ITEM_W) / 2.0f;   // 横は中央
	float y = Application::SCREEN_SIZE_Y * 0.55f;            // 縦は画面の 55% の位置
	menu_ = std::make_unique<Menu>(Menu::Layout{ {x, y}, ITEM_W, 60, 16 });

	menu_->AddItem(L"ゲーム開始", [this] { sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME); });
	menu_->AddItem(L"設定", [this] {  });
	menu_->AddItem(L"ゲーム終了", [] { Application::GetInstance().RequestExit(); });

}

void TitleScene::Update(void)
{
	menu_->Update();
}

// 描画
void TitleScene::Draw(void)
{
	menu_->Draw();
	DrawFormatString(0, 0, 0xffffff, L"タイトル");
}

void TitleScene::DrawTitle(void) const
{
}

void TitleScene::DrawMenu(void) const
{
}
