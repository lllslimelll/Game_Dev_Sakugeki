#pragma once

struct ContactPoint;
class CapsuleCollider;
class MeshCollider;
class CircleBoundaryCollider;

// 形状の組み合わせごとの詳細な当たり判定
namespace NarrowPhase
{
	// カプセルがメッシュにめり込んでいれば true。一番深いめり込みを out に入れる
	bool CapsuleVsMesh(const CapsuleCollider& capsule, const MeshCollider& mesh, ContactPoint& outContactPoint);

	// カプセルが円の境界からはみ出していれば true
	bool CapsuleVsBoundary(const CapsuleCollider& capsule, const CircleBoundaryCollider& boundary, ContactPoint& outContactPoint);
}