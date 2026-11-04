#include <algorithm>
#include "CollisionManager.h"

CollisionManager::CollisionManager(void)
	:
	colliders_()
{
}

CollisionManager::~CollisionManager(void)
{
}

void CollisionManager::Register(const Collider* collider)
{
	if (collider == nullptr) { return; }

	// 登録済みなら何もしない
	if (std::find(colliders_.begin(), colliders_.end(), collider) != colliders_.end()) { return; }

	// コライダーを追加
	colliders_.emplace_back(collider);
}

void CollisionManager::Unregister(const Collider* collider)
{
	// コライダーを削除
	colliders_.erase(
		std::remove(colliders_.begin(), colliders_.end(), collider),
		colliders_.end());
}

const std::vector<const Collider*>& CollisionManager::GetColliders(void) const
{
	return colliders_;
}