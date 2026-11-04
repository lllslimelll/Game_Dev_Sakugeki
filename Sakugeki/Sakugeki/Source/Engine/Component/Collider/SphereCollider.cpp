#include "../../../Engine/Common/Transform.h"
#include "SphereCollider.h"

SphereCollider::SphereCollider(
	TAG tag, const Transform* follow,
	const Vector3& localPos, float radius)
	:
	Collider(SHAPE::SPHERE, tag, follow),
	localPos_(localPos),
	radius_(radius)
{
}

SphereCollider::~SphereCollider(void)
{
}

const Vector3& SphereCollider::GetLocalPos(void) const
{
	return localPos_;
}

void SphereCollider::SetLocalPos(const Vector3& localPos)
{
	localPos_ = localPos;
}

Vector3 SphereCollider::GetPos(void) const
{
	return GetRotPos(localPos_);
}

float SphereCollider::GetRadius(void) const
{
	return radius_;
}

void SphereCollider::SetRadius(float radius)
{
	radius_ = radius;
}

Vector3 SphereCollider::GetPosPushBackAlongNormal(const MV1_COLL_RESULT_POLY& hitPoly, int maxTryCnt, float pushDistance) const
{
	// 追従先の自身のインスタンスをコピー生成
	Transform tmpTransform = *follow_;
	SphereCollider tmpSphere = *this;
	tmpSphere.SetFollow(&tmpTransform);

	// 衝突補正処理
	int tryCnt = 0;
	while (tryCnt < maxTryCnt)
	{
		// 球体と三角形の当たり判定
		if (!HitCheck_Sphere_Triangle(
			tmpSphere.GetPos().ToVECTOR(), tmpSphere.GetRadius(),
			hitPoly.Position[0], hitPoly.Position[1], hitPoly.Position[2]))
		{
			break;
		}

		// 衝突していたら法線方向に押し戻し
		tmpTransform.pos =
			tmpTransform.pos + (Vector3::GetVector3(hitPoly.Normal) * pushDistance);

		tryCnt++;
	}

	// 押し戻し座標を返す
	return tmpTransform.pos;
}

void SphereCollider::DrawDebug(int color)
{
	DrawSphere3D(GetPos().ToVECTOR(), radius_, 16, color, color, false);
}
