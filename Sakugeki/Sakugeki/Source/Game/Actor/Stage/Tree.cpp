#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Component/Mesh.h"
#include "../../../Engine/Component/MeshRenderer.h"
#include "../../../Engine/Component/Collider/CircleBoundaryCollider.h"
#include "../../../Engine/Component/Collider/MeshCollider.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Common/Transform.h"
#include "../../../Game/Common/CollisionLayer.h"
#include "Tree.h"

Tree::Tree(CollisionManager& collisionMng)
	:
	GameObject()
{
	AddComponent<Mesh>();
	// â~é¸ÇÃã´äEê¸ÉRÉâÉCÉ_
	//AddComponent<CircleBoundaryCollider>(collisionMng, Collider::TAG::WALL, BOUNDARY_RADIUS);
	AddComponent<MeshCollider>(collisionMng, CollisionLayer::TREE);
}

Tree::~Tree(void)
{
}

void Tree::Load(void)
{
	mesh_->SetHandle(
		resMng_.Load(ResourceManager::SRC::STAGE_GROUND).handleId_);

	resMng_.Load(ResourceManager::SRC::SHADER_VS_TREE);
	resMng_.Load(ResourceManager::SRC::SHADER_PS_TREE);
}

void Tree::Update(void)
{
}

void Tree::InitTransform(void)
{
	transform_.scl = Vector3::ONE;
	transform_.pos = Vector3::ZERO;

	transform_.Update();
}

void Tree::InitRenderer(void)
{
	AddComponent<MeshRenderer>(
		*mesh_,
		ResourceManager::SRC::SHADER_VS_TREE, ResourceManager::SRC::SHADER_PS_TREE);
}
