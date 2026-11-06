#include <cassert>
#include "../../Engine/Common/Transform.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../../Engine/Component/Collider/Collider.h"
#include "../../Engine/Component/Collider/CapsuleCollider.h"
#include "../../Engine/Component/Collider/PlaneCollider.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Component/Collider/ContactPoint.h"
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
	result.isSides = ResolveCollisions(); 

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

bool CharacterController::ResolveCollisions(void)
{
	Vector3& pos = gameObject_.GetTransform().pos;
	bool isResolved = false;

	for (int i = 0; i < MAX_RESOLVE_ITERATIONS; i++)
	{
		bool isHitAny = false;

		for (const Collider* hitCol : collisionMng_.GetColliders())
		{
			if (!hitCol->IsEnabled()) { continue; }

			// 自分のコライダーは無視する
			if (&hitCol->GetGameObject() == &gameObject_) { continue; }

			// 衝突していなければ無視する
			ContactPoint contactPoint;
			if (!collisionMng_.ComputePenetration(*capsule_, *hitCol, contactPoint)) { continue; }

			// めり込んだ分だけ押し戻す
			pos += contactPoint.normal * contactPoint.depth;
			isHitAny = true;
		}

		// どれにも当たらなくなったら終了
		if (!isHitAny) { break; }
		isResolved = true;
	}

	return isResolved;
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