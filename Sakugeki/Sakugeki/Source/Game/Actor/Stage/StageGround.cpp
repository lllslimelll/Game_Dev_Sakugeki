#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Component/MeshRendererComponent.h"
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Collider/ColliderPlane.h"
#include "../../../Engine/Collider/ColliderCircleBoundary.h" 
#include "StageGround.h"

StageGround::StageGround(void)
	:
	GameObject()
{
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
	AddComponent<MeshRendererComponent>(
		ResourceManager::SRC::SHADER_VS_GROUND, ResourceManager::SRC::SHADER_PS_GROUND);
}

void StageGround::InitCollider(void)
{
	// 平面コライダ（地面）
	ColliderPlane* colPlane =
		new ColliderPlane(ColliderBase::TAG::GROUND, &transform_);

	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::PLANE), colPlane);

	// 円周の境界線コライダ（移動制限用の壁）
	ColliderCircleBoundary* colBoundary =
		new ColliderCircleBoundary(ColliderBase::TAG::WALL, &transform_, BOUNDARY_RADIUS);
	ownColliders_.emplace(static_cast<int>(COLLIDER_TYPE::BOUNDARY), colBoundary);
}