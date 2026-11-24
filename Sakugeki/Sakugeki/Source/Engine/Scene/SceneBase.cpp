#include "../../Engine/Resource/ResourceManager.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/Scene/SceneManager.h"
#include "SceneBase.h"

SceneBase::SceneBase(void)
	: 
	resMng_(ResourceManager::GetInstance()),
	sceMng_(SceneManager::GetInstance()),
	input_(Input::GetInstance())
{
}

SceneBase::~SceneBase(void)
{
}
