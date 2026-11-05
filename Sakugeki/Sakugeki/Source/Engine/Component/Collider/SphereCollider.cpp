#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Component/Collider/SphereCollider.h"

SphereCollider::SphereCollider(
	GameObject& gameObject, CollisionManager& collisionMng, TAG tag,
	const Vector3& localPos, float radius)
	:
	Collider(gameObject, collisionMng, SHAPE::SPHERE, tag),
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

Vector3 SphereCollider::GetPosPushBackAlongNormal(
	const MV1_COLL_RESULT_POLY& hitPoly, int maxTryCnt, float pushDistance) const
{
	// 持ち主の回転を反映したオフセット
	const Vector3 basePos = GetTransform().pos;
	const Vector3 offset = GetPos() - basePos;
	// 位置
	Vector3 pos = basePos;
	// 衝突したポリゴンの法線ベクトル
	const Vector3 normal = Vector3::GetVector3(hitPoly.Normal);

	// 衝突補正処理
	for (int tryCnt = 0; tryCnt < maxTryCnt; tryCnt++)
	{
		if (!HitCheck_Sphere_Triangle(
			(pos + offset).ToVECTOR(), radius_,
			hitPoly.Position[0], hitPoly.Position[1], hitPoly.Position[2]))
		{
			break;
		}

		// 法線方向に押し戻す
		pos = pos + (normal * pushDistance);
	}

	return pos;
}

void SphereCollider::DrawDebug(int color)
{
	DrawSphere3D(GetPos().ToVECTOR(), radius_, 16, color, color, false);
}
