#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Debug/DebugDraw3D.h"
#include "../../../Engine/Component/Collider/SphereCollider.h"
#include "../../../Engine/Component/Collider/CollisionManager.h"

SphereCollider::SphereCollider(
	GameObject& gameObject, CollisionManager& collisionMng, int layer,
	const Vector3& localPos, float radius)
	:
	Collider(gameObject, collisionMng, SHAPE::SPHERE, layer),
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

void SphereCollider::AddDebugShape(unsigned int color) const
{
	DebugDraw3D::Sphere(GetPos(), radius_, color, DebugDraw3D::CATEGORY::COLLIDER, GetLayer());
}
