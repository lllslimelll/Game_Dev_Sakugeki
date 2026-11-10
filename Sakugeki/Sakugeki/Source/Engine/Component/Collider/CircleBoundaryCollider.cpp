#include <cmath>
#include "CircleBoundaryCollider.h"

CircleBoundaryCollider::CircleBoundaryCollider(
	GameObject& gameObject, CollisionManager& collisionMng, int layer, float radius)
	:
	Collider(gameObject, collisionMng, SHAPE::CIRCLE_BOUNDARY, layer),
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