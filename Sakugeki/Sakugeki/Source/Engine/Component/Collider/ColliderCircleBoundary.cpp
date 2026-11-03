#include <cmath>
#include "ColliderCircleBoundary.h"

ColliderCircleBoundary::ColliderCircleBoundary(
	TAG tag, const Transform* follow, float radius)
	:
	ColliderBase(SHAPE::CIRCLE_BOUNDARY, tag, follow),
	radius_(radius)
{
}

ColliderCircleBoundary::~ColliderCircleBoundary(void)
{
}

float ColliderCircleBoundary::GetRadius(void) const
{
	return radius_;
}

bool ColliderCircleBoundary::PushBackInside(Vector3& pos, float bodyRadius) const
{
	// 円の中心（追従先の座標）からの水平ベクトル
	const Vector3& center = follow_->pos;
	float dx = pos.x - center.x;
	float dz = pos.z - center.z;

	// 体の半径分だけ内側を限界にする
	float limit = radius_ - bodyRadius;
	if (limit < 0.0f) { limit = 0.0f; }

	// 二乗で比較して、外に出ているときだけ平方根を取る
	float dist2 = dx * dx + dz * dz;
	if (dist2 <= limit * limit)
	{
		return false;
	}

	// 中心から外向きの方向に沿って円周上へ押し戻す（壁沿いに自然に滑る）
	float dist = sqrtf(dist2);
	pos.x = center.x + dx / dist * limit;
	pos.z = center.z + dz / dist * limit;

	return true;
}