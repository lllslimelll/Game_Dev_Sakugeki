#include <DxLib.h>
#include "OptionScene.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Input/Input.h"

OptionScene::OptionScene(void) {}
OptionScene::~OptionScene(void) {}

void OptionScene::Init(void) 
{

}

void OptionScene::Update(void)
{
	if (input_.IsTriggered(Input::Command::CANCEL))
	{
		sceMng_.PopOverlay();
		return;
	}
}

void OptionScene::Draw(void)
{
}
