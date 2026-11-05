#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"
#include "../../../Engine/Component/Collider/PlaneCollider.h"
#include "../../../Engine/Component/Collider/CircleBoundaryCollider.h" 
#include "StageGround.h"

StageGround::StageGround(CollisionManager& collisionMng)
	:
	GameObject()
{
	// 平面コライダ
	AddComponent<PlaneCollider>(collisionMng, Collider::TAG::GROUND);
	// 円周の境界線コライダ
	AddComponent<CircleBoundaryCollider>(collisionMng, Collider::TAG::WALL, BOUNDARY_RADIUS);
}

StageGround::~StageGround(void)
{
}

void StageGround::Load(void)
{
	// モデル
	transform_.SetModel(
		resMng_.Load(ResourceManager::SRC::STAGE_GROUND).handleId_);

	// シェーダー
	resMng_.Load(ResourceManager::SRC::SHADER_VS_GROUND);
	resMng_.Load(ResourceManager::SRC::SHADER_PS_GROUND);
}

void StageGround::Update(void)
{
}

void StageGround::InitTransform(void)
{
	transform_.scl = Vector3::ONE;
	transform_.pos = Vector3::ZERO;

	transform_.Update();
}

void StageGround::InitRenderer(void)
{
	AddComponent<MeshRenderer>(
		ResourceManager::SRC::SHADER_VS_GROUND, ResourceManager::SRC::SHADER_PS_GROUND);
}