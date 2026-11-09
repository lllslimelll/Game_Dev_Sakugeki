#include <cmath>
#include "CircleBoundaryCollider.h"

CircleBoundaryCollider::CircleBoundaryCollider(
	GameObject& gameObject, CollisionManager& collisionMng, TAG tag, float radius)
	:
	Collider(gameObject, collisionMng, SHAPE::CIRCLE_BOUNDARY, tag),
	radius_(radius)
{
}

CircleBoundaryCollider::~CircleBoundaryCollider(void)
{
}

float CircleBoundaryCollider::GetRadius(void) const
{
	return radius_;
}