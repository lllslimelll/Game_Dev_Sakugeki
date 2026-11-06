#include <algorithm>
#include <cmath>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "CircleBoundaryCollider.h"
#include "ContactPoint.h"
#include "NarrowPhase.h"

namespace
{
	// 芯と三角形の距離がこれ未満なら、最近点から法線を作れないのでポリゴンの法線を使う
	constexpr float MIN_DISTANCE = 0.0001f;
}

bool NarrowPhase::CapsuleVsMesh(const CapsuleCollider& capsule, const MeshCollider& mesh, ContactPoint& outContactPoint)
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
		outContactPoint.normal = normal;
		outContactPoint.depth = depth;
		outContactPoint.other = &mesh;
		isHit = true;
	}

	// 検出したポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(hits);

	return isHit;
}

bool NarrowPhase::CapsuleVsBoundary(const CapsuleCollider& capsule, const CircleBoundaryCollider& boundary, ContactPoint& outContactPoint)
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

	const float dist = std::sqrt(dist2);
	outContactPoint.normal = Vector3(-dx / dist, 0.0f, -dz / dist);	// 中心へ向かう
	outContactPoint.depth = dist - limit;
	outContactPoint.other = &boundary;
	return true;
}