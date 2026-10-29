#include "../../../Engine/Resource/ResourceManager.h"
#include "../../../Engine/Renderer/Material.h"
#include "../../../Engine/Renderer/ModelRenderer.h"
#include "../../../Engine/Utility/AsoUtility.h"
#include "../../../Engine/Common/Transform.h"
#include "StageTree.h"

StageTree::StageTree(void)
	:
	ActorBase()
{
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
	material_ = std::make_unique<Material>(
		ResourceManager::SRC::SHADER_VS_TREE, 0,
		ResourceManager::SRC::SHADER_PS_TREE, 0);

	renderer_ = std::make_unique<ModelRenderer>(*material_, transform_.modelId);
}

void StageTree::InitCollider(void)
{
}