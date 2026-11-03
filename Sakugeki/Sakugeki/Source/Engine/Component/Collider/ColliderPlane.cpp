#include "ColliderPlane.h"

ColliderPlane::ColliderPlane(TAG tag, const Transform* follow)
	:
	ColliderBase(SHAPE::PLANE, tag, follow)
{
}

const float ColliderPlane::GetHeight() const
{
	return follow_->pos.y;
}
