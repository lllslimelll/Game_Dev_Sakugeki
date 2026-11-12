#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"
#include "../../../Engine/Component/Collider/MeshCollider.h"
#include "../../../Engine/Component/Collider/CircleBoundaryCollider.h" 
#include "../../../Game/Common/CollisionLayer.h"
#include "Ground.h"

Ground::Ground(CollisionManager& collisionMng)
	:
	GameObject()
{
	// メッシュコライダ
	AddComponent<MeshCollider>(collisionMng, CollisionLayer::GROUND);
}

Ground::~Ground(void)
{
}

void Ground::Load(void)
{
	// モデル
	transform_.SetModel(
		resMng_.Load(ResourceManager::SRC::STAGE_GROUND).handleId_);

	// シェーダー
	resMng_.Load(ResourceManager::SRC::SHADER_VS_GROUND);
	resMng_.Load(ResourceManager::SRC::SHADER_PS_GROUND);
}

void Ground::Update(void)
{
}

void Ground::InitTransform(void)
{
	transform_.scl = Vector3::ONE;
	transform_.pos = Vector3::ZERO;

	transform_.Update();
}

void Ground::InitRenderer(void)
{
	AddComponent<MeshRenderer>(
		ResourceManager::SRC::SHADER_VS_GROUND, ResourceManager::SRC::SHADER_PS_GROUND);
}