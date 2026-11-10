#include "../../../Engine/Common/Transform.h"
#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(GameObject& gameObject, CollisionManager& collisionMng, int layer)
	:
	Collider(gameObject, collisionMng, SHAPE::PLANE, layer)
{
}

const float PlaneCollider::GetHeight() const
{
	return GetTransform().pos.y;
}
