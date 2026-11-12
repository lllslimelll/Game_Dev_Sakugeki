#pragma once

struct ContactPoint;
struct RaycastHit;
class Vector3;
class CapsuleCollider;
class MeshCollider;
class CircleBoundaryCollider;
class SphereCollider;

// 形状の組み合わせごとの詳細な衝突判定
namespace NarrowPhase
{
	// 線分 x メッシュ
	bool LineVsMesh(const Vector3& start, const Vector3& end, const MeshCollider& mesh, RaycastHit& hitResult);

	// カプセル x メッシュ
	bool CapsuleVsMesh(const CapsuleCollider& capsule, const MeshCollider& mesh, ContactPoint& contactPoint);

	// カプセル x 円周の境界
	bool CapsuleVsBoundary(const CapsuleCollider& capsule, const CircleBoundaryCollider& boundary, ContactPoint& contactPoint);

	// 球 x メッシュ
	bool SphereVsMesh(const SphereCollider& sphere, const MeshCollider& mesh, ContactPoint& contactPoint);
}