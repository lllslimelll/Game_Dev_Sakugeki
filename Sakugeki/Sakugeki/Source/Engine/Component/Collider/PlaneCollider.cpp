#include "PlaneCollider.h"

PlaneCollider::PlaneCollider(TAG tag, const Transform* follow)
	:
	Collider(SHAPE::PLANE, tag, follow)
{
}

const float PlaneCollider::GetHeight() const
{
	return follow_->pos.y;
}
