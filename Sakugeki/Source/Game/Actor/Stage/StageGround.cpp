#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Renderer/Material.h"
#include "../../../Engine/Renderer/ModelRenderer.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../Common/Transform.h"
#include "../../Collider/ColliderPlane.h"
#include "../../Collider/ColliderCircleBoundary.h" 
#include "StageGround.h"

StageGround::StageGround(void)
	:
	ActorBase()
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
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = AsoUtility::VECTOR_ZERO;

	transform_.Update();
}

void StageGround::InitRenderer(void)
{
	material_ = std::make_unique<Material>(
		ResourceManager::SRC::SHADER_VS_GROUND, 0,
		ResourceManager::SRC::SHADER_PS_GROUND, 0);

	renderer_ = std::make_unique<ModelRenderer>(*material_, transform_.modelId);
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