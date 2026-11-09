#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"
#include "../../../Engine/Component/Collider/PlaneCollider.h"
#include "../../../Engine/Component/Collider/CircleBoundaryCollider.h" 
#include "Ground.h"

Ground::Ground(CollisionManager& collisionMng)
	:
	GameObject()
{
	// 平面コライダ
	AddComponent<PlaneCollider>(collisionMng, Collider::TAG::GROUND);
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