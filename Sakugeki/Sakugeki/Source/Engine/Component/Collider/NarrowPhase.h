#pragma once

struct ContactPoint;
class CapsuleCollider;
class MeshCollider;
class CircleBoundaryCollider;
class SphereCollider;

// 形状の組み合わせごとの詳細な当たり判定
namespace NarrowPhase
{
	// カプセルがメッシュにめり込んでいれば true。一番深いめり込みを outContactPoint に入れる
	bool CapsuleVsMesh(const CapsuleCollider& capsule, const MeshCollider& mesh, ContactPoint& outContactPoint);

	// カプセルが円の境界からはみ出していれば true
	bool CapsuleVsBoundary(const CapsuleCollider& capsule, const CircleBoundaryCollider& boundary, ContactPoint& outContactPoint);

	// 球がメッシュにめり込んでいれば true。一番深いめり込みを outContactPoint に入れる
	bool SphereVsMesh(const SphereCollider& sphere, const MeshCollider& mesh, ContactPoint& outContactPoint);
}