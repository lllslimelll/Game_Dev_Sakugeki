#include <DxLib.h>
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Debug/DebugDraw3D.h"
#include "MeshCollider.h"
#include "CapsuleCollider.h"

CapsuleCollider::CapsuleCollider(
    GameObject& gameObject, CollisionManager& collisionMng, int layer,
    const Vector3& localPosTop, const Vector3& localPosDown, float radius)
    :
    Collider(gameObject, collisionMng, SHAPE::CAPSULE, layer),
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

void CapsuleCollider::AddDebugShape(unsigned int color) const
{
    DebugDraw3D::Capsule(GetPosTop(), GetPosDown(), radius_, color, DebugDraw3D::CATEGORY::COLLIDER, GetLayer());
}