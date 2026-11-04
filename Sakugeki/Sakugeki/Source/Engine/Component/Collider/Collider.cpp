#include "../../../Engine/Common/Transform.h"
#include "Collider.h"

Collider::Collider(SHAPE shape, TAG tag, const Transform* follow)
	:
	shape_(shape),
	tag_(tag),
	follow_(follow),
	isValid_(true)
{
}

Collider::~Collider(void)
{
}

void Collider::Draw(void)
{
	int color = COLOR_INVALID;

	if (isValid_)
	{
		color = COLOR_VALID;
	}

	DrawDebug(color);
}

void Collider::SetFollow(Transform* follow)
{
	follow_ = follow;
}

Vector3 Collider::GetRotPos(const Vector3& localPos) const
{
	// 追従相手の回転に合わせて指定ローカル座標を回転し、
	// 基準座標に加えることでワールド座標へ変換
	Vector3 localRotPos = follow_->quaRot.Rotate(localPos);

	return follow_->pos + localRotPos;
}