#include "../../Collider/ColliderBase.h"
#include "../../../Engine/Common/Transform.h"
#include "StageGround.h"
#include "StageTree.h"
#include "StageManager.h"

StageManager::StageManager(void)
{
}

StageManager::~StageManager(void)
{
}

void StageManager::Load(void)
{
	ground_ = std::make_unique<StageGround>();
	tree_ = std::make_unique<StageTree>();

	ground_->Load();
	tree_->Load();
}

void StageManager::Init(void)
{
	ground_->Init();
	tree_->Init();
}

void StageManager::Update(void)
{
	ground_->Update();
	tree_->Update();
}

void StageManager::Draw(void)
{
	ground_->Draw();
	tree_->Draw();
}

void StageManager::Release(void)
{
	if (ground_) { ground_->Release(); }
	if (tree_) { tree_->Release(); }
}

std::vector<const ColliderBase*> StageManager::GetColliders(void) const
{
	std::vector<const ColliderBase*> ret;

	for (const auto& [type, col] : ground_->GetOwnColliders())
	{
		ret.emplace_back(col);
	}
	for (const auto& [type, col] : tree_->GetOwnColliders())
	{
		ret.emplace_back(col);
	}

	return ret;
}

std::vector<int> StageManager::GetModelIds(void) const
{
	return {
		ground_->GetTransform().modelId,
		tree_->GetTransform().modelId
	};
}