#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Component/Collider/CircleBoundaryCollider.h"
#include "../../../Engine/Component/Collider/MeshCollider.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Common/Transform.h"
#include "StageTree.h"

StageTree::StageTree(CollisionManager& collisionMng)
	:
	GameObject()
{
	// â~é¸ÇÃã´äEê¸ÉRÉâÉCÉ_
	//AddComponent<CircleBoundaryCollider>(collisionMng, Collider::TAG::WALL, BOUNDARY_RADIUS);
	AddComponent<MeshCollider>(collisionMng, Collider::TAG::TREE);
}

StageTree::~StageTree(void)
{
}

void StageTree::Load(void)
{
	transform_.SetModel(
		resMng_.Load(ResourceManager::SRC::STAGE_TREE).handleId_);

	resMng_.Load(ResourceManager::SRC::SHADER_VS_TREE);
	resMng_.Load(ResourceManager::SRC::SHADER_PS_TREE);
}

void StageTree::Update(void)
{
}

void StageTree::InitTransform(void)
{
	transform_.scl = Vector3::ONE;
	transform_.pos = Vector3::ZERO;

	transform_.Update();
}

void StageTree::InitRenderer(void)
{
	AddComponent<MeshRenderer>(
		ResourceManager::SRC::SHADER_VS_TREE, ResourceManager::SRC::SHADER_PS_TREE);
}
