#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "../../Engine/Audio/Audio.h"
#include "../../Engine/Scene/SceneManager.h"
#include "DeadScene.h"


DeadScene::DeadScene()
	:
	SceneBase()
{
}

DeadScene::~DeadScene()
{
}

void DeadScene::Init(void)
{
}

void DeadScene::Update(void)
{
	UpdateMenu();
}

void DeadScene::Draw(void)
{
	DrawMenu();
}

void DeadScene::UpdateMenu(void)
{
    switch (menu_)
    {
    case MENU::RETRY:
        sceMng_.ChangeScene(SceneManager::SCENE_ID::GAME);
        break;
    case MENU::TITLE:
        sceMng_.ChangeScene(SceneManager::SCENE_ID::TITLE);
        break;
    default: break;
    }
}

void DeadScene::DrawMenu(void)
{
}
