#include "../../Engine/Resource/ResourceManager.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Common/Transform.h"
#include "../../Engine/Utility/AsoUtility.h"
#include "SkyBox.h"

SkyBox::SkyBox()
	:
	GameObject()
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::Load(void)
{
	// モデル読み込み
	//transform_.SetModel(resMng_.Load(		
	//	ResourceManager::SRC::SKY_DOME).handleId_);
}

void SkyBox::Update(void)
{

}

void SkyBox::Draw(void)
{

}

void SkyBox::InitTransform(void)
{
}

void SkyBox::InitRenderer()
{
}

void SkyBox::InitPost(void)
{
}