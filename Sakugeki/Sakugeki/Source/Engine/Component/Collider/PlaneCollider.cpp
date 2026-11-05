#include "../../../Engine/Common/Transform.h"
#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(GameObject& gameObject, CollisionManager& collisionMng, TAG tag)
	:
	Collider(gameObject, collisionMng, SHAPE::PLANE, tag)
{
}

const float PlaneCollider::GetHeight() const
{
	return GetTransform().pos.y;
}
