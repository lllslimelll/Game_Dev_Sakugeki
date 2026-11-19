#include <numbers>
#include <cmath>
#include "../../../Engine/Debug/DebugDraw3D.h"
#include "CircleBoundaryCollider.h"

namespace
{
    constexpr int CIRCLE_SEGMENTS = 64;		// 円を何本の線で描くか
}

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

void CircleBoundaryCollider::AddDebugShape(unsigned int color) const
{
    // 持ち主の位置を中心に、水平な円を描く
    const Vector3 center = GetTransform().pos;
    const float step = 2.0f * std::numbers::pi_v<float> / CIRCLE_SEGMENTS;

    for (int i = 0; i < CIRCLE_SEGMENTS; i++)
    {
        const float a0 = step * i;
        const float a1 = step * (i + 1);
        const Vector3 p0 = center + Vector3(std::cos(a0) * radius_, 0.0f, std::sin(a0) * radius_);
        const Vector3 p1 = center + Vector3(std::cos(a1) * radius_, 0.0f, std::sin(a1) * radius_);
        DebugDraw3D::Line(p0, p1, color, DebugDraw3D::CATEGORY::COLLIDER, GetLayer());
    }
}
