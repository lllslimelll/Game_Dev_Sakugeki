#include <DxLib.h>
#include "../../../Engine/Common/Transform.h"
#include "MeshCollider.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(
    GameObject& gameObject, CollisionManager& collisionMng, TAG tag,
    const Vector3& localPosTop, const Vector3& localPosDown, float radius)
    :
    Collider(gameObject, collisionMng, SHAPE::CAPSULE, tag),
    localPosTop_(localPosTop),
    localPosDown_(localPosDown),
    radius_(radius)
{
}

CapsuleCollider::~CapsuleCollider(void)
{
}

const Vector3& CapsuleCollider::GetLocalPosTop(void) const
{
    return localPosTop_;
}

const Vector3& CapsuleCollider::GetLocalPosDown(void) const
{
    return localPosDown_;
}

void CapsuleCollider::SetLocalPosTop(const Vector3& pos)
{
    localPosTop_ = pos;
}

void CapsuleCollider::SetLocalPosDown(const Vector3& pos)
{
    localPosDown_ = pos;
}

Vector3 CapsuleCollider::GetPosTop(void) const
{
    return GetRotPos(localPosTop_);
}

Vector3 CapsuleCollider::GetPosDown(void) const
{
	return GetRotPos(localPosDown_);
}

float CapsuleCollider::GetRadius(void) const
{
    return radius_;
}

void CapsuleCollider::SetRadius(float radius)
{
    radius_ = radius;
}

float CapsuleCollider::GetHeight(void) const
{
    return localPosTop_.y;
}

Vector3 CapsuleCollider::GetCenter(void) const
{
    Vector3 top = GetPosTop();
    Vector3 down = GetPosDown();

    Vector3 diff = top - down;
    return down + (diff * 0.5f);
}

void CapsuleCollider::DrawDebug(int color)
{
    // 上の球体
    Vector3 pos1 = GetPosTop();
    DrawSphere3D(pos1.ToVECTOR(), radius_, 5, color, color, false);

    // 下の球体
    Vector3 pos2 = GetPosDown();
    DrawSphere3D(pos2.ToVECTOR(), radius_, 5, color, color, false);

    Vector3 dir, start, end;

    // 球体を繋ぐ線(X+)
    dir = GetTransform().GetRight();
    start = pos1 + (dir * radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(X-)
    dir = GetTransform().GetLeft();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z+)
    dir = GetTransform().GetForward();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // 球体を繋ぐ線(Z-)
    dir = GetTransform().GetBack();
    start = pos1 + (dir *radius_);
    end = pos2 + (dir * radius_);
    DrawLine3D(start.ToVECTOR(), end.ToVECTOR(), color);

    // カプセルの中心
    DrawSphere3D(GetCenter().ToVECTOR(), 5.0f, 10, color, color, true);
}
