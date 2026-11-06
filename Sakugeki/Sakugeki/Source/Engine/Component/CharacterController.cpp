#include <cassert>
#include "../../Engine/Common/Transform.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../../Engine/Component/Collider/Collider.h"
#include "../../Engine/Component/Collider/CapsuleCollider.h"
#include "../../Engine/Component/Collider/CircleBoundaryCollider.h"
#include "../../Engine/Component/Collider/MeshCollider.h"
#include "../../Engine/Component/Collider/PlaneCollider.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Component/CharacterController.h"

CharacterController::CharacterController(GameObject& gameObject, const CollisionManager& collisionMng)
	:
	Component(gameObject),
	collisionMng_(collisionMng),
	capsule_(nullptr),
	isGrounded_(false)
{
}

CharacterController::~CharacterController(void)
{
}

void CharacterController::Init(void)
{
	capsule_ = gameObject_.GetComponent<CapsuleCollider>();
	assert(capsule_ != nullptr && "CharacterController には CapsuleCollider が必要です");
}

CharacterController::MoveResult CharacterController::Move(const Vector3& delta)
{
	MoveResult result;
	Transform& transform = gameObject_.GetTransform();

	// 水平方向の移動と衝突
	transform.pos += Vector3(delta.x, 0.0f, delta.z);
	const bool isHitBoundary = CollideBoundary();
	const bool isHitModel = CollideModel();
	result.isSides = isHitBoundary || isHitModel;

	// 垂直方向の移動と接地
	transform.pos += Vector3(0.0f, delta.y, 0.0f);
	result.isBelow = CollideGround(delta.y > 0.0f);

	isGrounded_ = result.isBelow;
	return result;
}

bool CharacterController::IsGrounded(void) const
{
	return isGrounded_;
}

bool CharacterController::CollideBoundary(void)
{
	Vector3& pos = gameObject_.GetTransform().pos;

	// 自分の体の半径
	const float bodyRadius = capsule_->GetRadius();

	bool isHit = false;
	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (!hitCol->IsEnabled()) continue;
		if (hitCol->GetShape() != Collider::SHAPE::CIRCLE_BOUNDARY) continue;

		const auto* boundary = static_cast<const CircleBoundaryCollider*>(hitCol);
		if (boundary->PushBackInside(pos, bodyRadius)) { isHit = true; }
	}
	return isHit;
}

bool CharacterController::CollideModel(void)
{
	Transform& transform = gameObject_.GetTransform();

	// 押し戻し関数は結果を返さないので、位置が変わったかで判定する
	const Vector3 prevPos = transform.pos;

	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (!hitCol->IsEnabled()) continue;
		if (hitCol->GetShape() != Collider::SHAPE::MESH) continue;

		const auto* model = static_cast<const MeshCollider*>(hitCol);

		// 指定された回数と距離で三角形の法線方向に押し戻す
		capsule_->PushBackAlongNormal(
			model, transform,
			CNT_TRY_COLLISION, DIS_COLLISION_BACK,
			true,	// ブラックリスト使用許可
			false	// ホワイトリスト使用許可
		);
	}
	return (transform.pos - prevPos).SquareMagnitude() > 0.0f;
}

bool CharacterController::CollideGround(bool isRising)
{
	// 上昇中は判定しない
	if (isRising) { return false; }

	Vector3& pos = gameObject_.GetTransform().pos;

	bool isHit = false;
	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (!hitCol->IsEnabled()) continue;
		if (hitCol->GetShape() != Collider::SHAPE::PLANE) continue;

		const auto* plane = static_cast<const PlaneCollider*>(hitCol);
		const float height = plane->GetHeight();

		if (pos.y <= height)
		{
			pos.y = height;
		
			isHit = true;
		}
	}
	return isHit;
}