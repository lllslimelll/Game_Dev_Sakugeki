#include <cassert>
#include <cmath>
#include "../../Engine/Math/UtilityMath.h"
#include "../../Engine/Common/Transform.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../../Engine/Component/Collider/Collider.h"
#include "../../Engine/Component/Collider/CapsuleCollider.h"
#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Component/Collider/ContactPoint.h"
#include "../../Engine/Component/Collider/RaycastHit.h"
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
	const float prevY = transform.pos.y; // 動く前の高さ
	transform.pos += Vector3(0.0f, delta.y, 0.0f);
	result.isBelow = CollideGround(prevY, delta.y > 0.0f);

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
		bool isPushedSide = false;

		for (const Collider* hitCol : collisionMng_.GetColliders())
		{
			// コライダーが無効・自分のコライダー・当たらないレイヤーなら無視する
			if (!hitCol->IsEnabled() ||
				&hitCol->GetGameObject() == &gameObject_ ||
				!collisionMng_.CanCollide(*capsule_, *hitCol)) 
			{ 
				continue; 
			}

			// 衝突していなければ無視する
			ContactPoint contactPoint;
			if (!collisionMng_.ComputePenetration(*capsule_, *hitCol, contactPoint)) { continue; }

			if (IsWalkable(contactPoint.normal))
			{
				// 床の向きなら、真上にだけ押し戻す
				pos.y += contactPoint.depth / contactPoint.normal.y;
			}
			else
			{
				// 壁や天井は、法線の向きに押し戻す
				pos += contactPoint.normal * contactPoint.depth;
				isPushedSide = true;
			}
		}

		// どれにも当たらなくなったら終了
		if (!isPushedSide) { break; }
		isResolved = true;
	}

	return isResolved;
}

bool CharacterController::CollideGround(float prevY, bool isRising)
{
	// 上昇中は判定しない
	if (isRising) { return false; }

	Vector3& pos = gameObject_.GetTransform().pos;

	// 動く前の高さの少し上から、今の足元（接地中なら少し下）までレイを飛ばす
	const float snap = isGrounded_ ? GROUND_SNAP : 0.0f;
	const Vector3 start(pos.x, prevY + GROUND_RAY_UP, pos.z);
	const Vector3 end(pos.x, pos.y - snap, pos.z);

	// 自分の体と同じ相手（地面・壁・木）を調べる
	const CollisionManager::LayerMask mask = collisionMng_.GetCollisionMask(capsule_->GetLayer());

	RaycastHit hit;
	if (!collisionMng_.Linecast(start, end, mask, hit, &gameObject_)) { return false; }

	// 急すぎる坂には立てない
	if (!IsWalkable(hit.normal)) { return false; }

	// 足元を地面の高さに合わせる
	pos.y = hit.point.y;
	return true;
}

bool CharacterController::IsWalkable(const Vector3& normal)
{
	// 法線と真上の角度が SLOPE_LIMIT_DEG 以内なら立てる
	static const float minNormalY = std::cos(UtilityMath::DegToRadF(SLOPE_LIMIT_DEG));
	return normal.y >= minNormalY;
}