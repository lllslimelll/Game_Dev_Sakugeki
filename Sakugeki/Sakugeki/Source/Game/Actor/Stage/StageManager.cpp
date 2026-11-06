#include "../../../Engine/Common/Transform.h"
#include "StageGround.h"
#include "StageTree.h"
#include "StageManager.h"

StageManager::StageManager(CollisionManager& collisionMng)
{
	ground_ = std::make_unique<StageGround>(collisionMng);
	tree_ = std::make_unique<StageTree>(collisionMng);
}

StageManager::~StageManager()
{
}

void StageManager::Load()
{
	ground_->Load();
	tree_->Load();
}

void StageManager::Init()
{
	ground_->Init();
	tree_->Init();
}

void StageManager::Update()
{
	ground_->Update();
	tree_->Update();
}

void StageManager::Draw()
{
	ground_->Draw();
	tree_->Draw();
}

void StageManager::Release()
{
	if (ground_) { ground_->Release(); }
	if (tree_) { tree_->Release(); }
}

std::vector<int> StageManager::GetModelIds() const
{
	return {
		ground_->GetTransform().modelId,
		tree_->GetTransform().modelId
	};
}