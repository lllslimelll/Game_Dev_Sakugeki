#include <algorithm>
#include <cassert>
#include "Collider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "CircleBoundaryCollider.h"
#include "SphereCollider.h"
#include "ContactPoint.h"
#include "NarrowPhase.h"
#include "CollisionManager.h"

CollisionManager::CollisionManager(void)
	:
	colliders_(),
	layerMasks_()
{
	// 衝突表を初期化（すべてのレイヤー同士が衝突する）
	layerMasks_.fill(0xFFFFFFFFu);
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

bool CollisionManager::ComputePenetration(const Collider& a, const Collider& b, ContactPoint& outContactPoint) const
{
	using SHAPE = Collider::SHAPE;
	const SHAPE shapeA = a.GetShape();
	const SHAPE shapeB = b.GetShape();

	// 形状の組み合わせによって、当たり判定を行う
	if (shapeA == SHAPE::CAPSULE && shapeB == SHAPE::MESH)
	{
		return NarrowPhase::CapsuleVsMesh(
			static_cast<const CapsuleCollider&>(a), static_cast<const MeshCollider&>(b), outContactPoint);
	}
	if (shapeA == SHAPE::CAPSULE && shapeB == SHAPE::CIRCLE_BOUNDARY)
	{
		return NarrowPhase::CapsuleVsBoundary(
			static_cast<const CapsuleCollider&>(a), static_cast<const CircleBoundaryCollider&>(b), outContactPoint);
	}
	if (shapeA == SHAPE::SPHERE && shapeB == SHAPE::MESH)
	{
		return NarrowPhase::SphereVsMesh(
			static_cast<const SphereCollider&>(a), static_cast<const MeshCollider&>(b), outContactPoint);
	}

	return false;
}

void CollisionManager::SetLayerCollision(int a, int b, bool isCollide)
{
	assert(0 <= a && a < MAX_LAYERS && 0 <= b && b < MAX_LAYERS);

	const std::uint32_t bitA = 1u << a;
	const std::uint32_t bitB = 1u << b;

	// 衝突表を対象にする
	if (isCollide)
	{
		layerMasks_[a] |= bitB;
		layerMasks_[b] |= bitA;
	}
	else
	{
		layerMasks_[a] &= ~bitB;
		layerMasks_[b] &= ~bitA;
	}
}

bool CollisionManager::IsLayerCollide(int a, int b) const
{
	return (layerMasks_[a] & (1u << b)) != 0;
}

bool CollisionManager::CanCollide(const Collider& a, const Collider& b) const
{
	return IsLayerCollide(a.GetLayer(), b.GetLayer());
}
