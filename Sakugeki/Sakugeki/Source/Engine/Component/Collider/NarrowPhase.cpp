#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "CircleBoundaryCollider.h"
#include "SphereCollider.h" 
#include "ContactPoint.h"
#include "RaycastHit.h"
#include "NarrowPhase.h"

namespace
{
	// 芯と三角形の距離がこれ未満なら、最近点から法線を作れないのでポリゴンの法線を使う
	constexpr float MIN_DISTANCE = 0.0001f;
}

bool NarrowPhase::LineVsMesh(const Vector3& start, const Vector3& end, const MeshCollider& mesh, RaycastHit& hitResult)
{
	// 線分と当たったポリゴンをすべて集める
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_LineDim(
		mesh.GetTransform().modelId, -1, start.ToVECTOR(), end.ToVECTOR());

	bool isHit = false;
	float minDist = 0.0f;

	for (int i = 0; i < hits.HitNum; i++)
	{
		const MV1_COLL_RESULT_POLY& poly = hits.Dim[i];

		// 除外フレームは無視する
		if (mesh.IsExcludeFrame(poly.FrameIndex)) { continue; }

		// 始点に一番近い当たりを残す
		const Vector3 point = Vector3::GetVector3(poly.HitPosition);
		const float dist = static_cast<float>(Vector3::Distance(start, point));
		if (isHit && dist >= minDist) { continue; }

		minDist = dist;
		hitResult.point = point;
		hitResult.normal = Vector3::GetVector3(poly.Normal);
		hitResult.distance = dist;
		hitResult.other = &mesh;

		isHit = true;
	}

	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return isHit;
}

bool NarrowPhase::CapsuleVsMesh(const CapsuleCollider& capsule, const MeshCollider& mesh, ContactPoint& contactPoint)
{
	const VECTOR top = capsule.GetPosTop().ToVECTOR();
	const VECTOR down = capsule.GetPosDown().ToVECTOR();
	const float radius = capsule.GetRadius();

	// 当たっている可能性のあるポリゴンを集める
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Capsule(
		mesh.GetTransform().modelId, -1, top, down, radius);

	bool isHit = false;
	float maxDepth = 0.0f;

	for (int i = 0; i < hits.HitNum; i++)
	{
		const MV1_COLL_RESULT_POLY& poly = hits.Dim[i];

		// 除外フレームは無視する
		if (mesh.IsExcludeFrame(poly.FrameIndex)) { continue; }

		// カプセルの芯（線分）と三角形の最も近い点の組
		SEGMENT_TRIANGLE_RESULT result;
		Segment_Triangle_Analyse(&top, &down,
			&poly.Position[0], &poly.Position[1], &poly.Position[2], &result);

		const float dist = std::sqrt(result.Seg_Tri_MinDist_Square);
		const float depth = radius - dist;

		// 当たっていない、または今までより浅いなら無視する
		if (depth <= maxDepth) { continue; }

		// 三角形側の点 → 芯側の点 の向きが、押し戻す方向
		Vector3 normal;
		if (dist > MIN_DISTANCE)
		{
			normal = (Vector3::GetVector3(result.Seg_MinDist_Pos)
				- Vector3::GetVector3(result.Tri_MinDist_Pos)) / dist;
		}
		else
		{
			// 芯が三角形を貫いているので、ポリゴンの法線を使う
			normal = Vector3::GetVector3(poly.Normal);
		}

		maxDepth = depth;
		contactPoint.point = Vector3::GetVector3(result.Tri_MinDist_Pos);
		contactPoint.normal = normal;
		contactPoint.depth = depth;
		contactPoint.other = &mesh;
		isHit = true;
	}

	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return isHit;
}

bool NarrowPhase::CapsuleVsBoundary(const CapsuleCollider& capsule, const CircleBoundaryCollider& boundary, ContactPoint& contactPoint)
{
	// XZ 平面で判定する（境界は、無限に高い円筒の壁）
	const Vector3& pos = capsule.GetTransform().pos;
	const Vector3& center = boundary.GetTransform().pos;
	const float dx = pos.x - center.x;
	const float dz = pos.z - center.z;

	// 体の半径分だけ内側を限界にする
	const float limit = (std::max)(0.0f, boundary.GetRadius() - capsule.GetRadius());

	// 二乗で比較して、外に出ているときだけ平方根を取る
	const float dist2 = dx * dx + dz * dz;
	if (dist2 <= limit * limit) { return false; }

	const float wallRadius = boundary.GetRadius();
	const float dist = std::sqrt(dist2);
	contactPoint.point = Vector3(center.x + dx / dist * wallRadius, pos.y, center.z + dz / dist * wallRadius);
	contactPoint.normal = Vector3(-dx / dist, 0.0f, -dz / dist);	// 中心へ向かう
	contactPoint.depth = dist - limit;
	contactPoint.other = &boundary;
	return true;
}

bool NarrowPhase::SphereVsMesh(const SphereCollider& sphere, const MeshCollider& mesh, ContactPoint& contactPoint)
{
	const VECTOR center = sphere.GetPos().ToVECTOR();
	const float radius = sphere.GetRadius();

	// 当たっている可能性のあるポリゴンを集める
	MV1_COLL_RESULT_POLY_DIM hits = MV1CollCheck_Sphere(
		mesh.GetTransform().modelId, -1, center, radius);

	bool isHit = false;
	float maxDepth = 0.0f;

	for (int i = 0; i < hits.HitNum; i++)
	{
		const MV1_COLL_RESULT_POLY& poly = hits.Dim[i];

		// 除外フレームは無視する
		if (mesh.IsExcludeFrame(poly.FrameIndex)) { continue; }

		// 球の中心と三角形の最も近い点
		TRIANGLE_POINT_RESULT result;
		Triangle_Point_Analyse(
			&poly.Position[0], &poly.Position[1], &poly.Position[2], &center, &result);

		const float dist = std::sqrt(result.Tri_Pnt_MinDist_Square);
		const float depth = radius - dist;

		// 当たっていない、または今までより浅いなら無視する
		if (depth <= maxDepth) { continue; }

		// 三角形側の点 → 球の中心 の向きが、押し戻す方向
		Vector3 normal;
		if (dist > MIN_DISTANCE)
		{
			normal = (Vector3::GetVector3(center) - Vector3::GetVector3(result.Tri_MinDist_Pos)) / dist;
		}
		else
		{
			// 中心が三角形の上にあるので、ポリゴンの法線を使う
			normal = Vector3::GetVector3(poly.Normal);
		}

		maxDepth = depth;
		contactPoint.point = Vector3::GetVector3(result.Tri_MinDist_Pos);
		contactPoint.normal = normal;
		contactPoint.depth = depth;
		contactPoint.other = &mesh;
		isHit = true;
	}

	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return isHit;
}
