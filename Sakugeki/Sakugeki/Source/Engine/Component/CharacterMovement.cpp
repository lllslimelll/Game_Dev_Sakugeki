#include <algorithm>
#include "../../Engine/Common/Transform.h"
#include "../../Engine/Math/Quaternion.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/Collider/Collider.h"
#include "../../Engine/Collider/CapsuleCollider.h"
#include "../../Engine/Collider/CircleBoundaryCollider.h"
#include "../../Engine/Collider/ModelCollider.h"
#include "../../Engine/Collider/PlaneCollider.h"
#include "../../Engine/Collider/CollisionManager.h"
#include "../../Engine/Component/CharacterMovement.h"

CharacterMovement::CharacterMovement(GameObject& gameObject, Transform& transform,
	const CollisionManager& collisionMng)
	:
	Component(gameObject),
	transform_(transform),
	collisionMng_(collisionMng),
	capsule_(nullptr),
	maxWalkSpeed_(0.0f),
	jumpSpeed_(0.0f),
	velocity_(Vector3::ZERO),
	moveInput_(Vector3::ZERO),
	facingDir_(Vector3::ZERO),
	isJumpRequested_(false),
	isGrounded_(false)
{
}

CharacterMovement::~CharacterMovement(void)
{
}

void CharacterMovement::Update(void)
{
	const float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 水平方向の速度
	Vector3 input(moveInput_.x, 0.0f, moveInput_.z);
	if (input.SquareMagnitude() > 1.0f) { input = input.Normalized(); }
	velocity_.x = input.x * maxWalkSpeed_;
	velocity_.z = input.z * maxWalkSpeed_;

	// 入力があれば、その方向を向く
	if (!input.IsZero()) 
	{
		facingDir_ = input.Normalized();
	}

	// ジャンプ（接地しているときだけ）
	if (isJumpRequested_ && isGrounded_)
	{
		velocity_.y = jumpSpeed_;
		isGrounded_ = false;
	}

	// 移動方向へ向きを補間
	RotateToFacing();

	// 重力
	ApplyGravity(deltaTime);

	// 水平方向の移動と衝突
	transform_.pos += Vector3(velocity_.x, 0.0f, velocity_.z) * deltaTime;
	CollideBoundary();
	CollideModel();

	// 垂直方向の移動と接地
	transform_.pos += Vector3(0.0f, velocity_.y, 0.0f) * deltaTime;
	CollideGround();

	// 指示はこのフレームで消費する
	moveInput_ = Vector3::ZERO;
	isJumpRequested_ = false;
}

void CharacterMovement::AddMoveInput(const Vector3& worldDir)
{
	moveInput_ += worldDir;
}

void CharacterMovement::Jump(void)
{
	isJumpRequested_ = true;
}

void CharacterMovement::SetCapsule(const CapsuleCollider* capsule)
{
	capsule_ = capsule;
}

void CharacterMovement::SetMaxWalkSpeed(float speed)
{
	maxWalkSpeed_ = speed;
}

void CharacterMovement::SetJumpSpeed(float speed)
{
	jumpSpeed_ = speed;
}

const Vector3& CharacterMovement::GetVelocity(void) const
{
	return velocity_;
}

bool CharacterMovement::IsGrounded(void) const
{
	return isGrounded_;
}

bool CharacterMovement::IsMoving(void) const
{
	return !Vector3(velocity_.x, 0.0f, velocity_.z).IsZero();
}

void CharacterMovement::RotateToFacing(void)
{
	if (facingDir_.IsZero()) { return; }

	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(facingDir_);

	// 回転の補間
	transform_.quaRot = Quaternion::Slerp(transform_.quaRot, goalRot, ROTATE_RATE);
}

void CharacterMovement::ApplyGravity(float deltaTime)
{
	velocity_.y -= GRAVITY * deltaTime;

	// 落下速度の上限
	velocity_.y = (std::max)(velocity_.y, -FALL_SPEED_MAX);
}

void CharacterMovement::CollideBoundary(void)
{
	// 自分の体の半径
	float bodyRadius = (capsule_ != nullptr) ? capsule_->GetRadius() : 0.0f;

	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (hitCol->GetShape() != Collider::SHAPE::CIRCLE_BOUNDARY) continue;

		const auto* boundary = static_cast<const CircleBoundaryCollider*>(hitCol);
		boundary->PushBackInside(transform_.pos, bodyRadius);
	}
}

void CharacterMovement::CollideModel(void)
{
	if (capsule_ == nullptr) { return; }

	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (hitCol->GetShape() != Collider::SHAPE::MODEL) continue;

		const auto* model = static_cast<const ModelCollider*>(hitCol);

		// 指定された回数と距離で三角形の法線方向に押し戻す
		capsule_->PushBackAlongNormal(
			model, transform_,
			CNT_TRY_COLLISION, DIS_COLLISION_BACK,
			true,	// ブラックリスト使用許可
			false	// ホワイトリスト使用許可
		);
	}
}

void CharacterMovement::CollideGround(void)
{
	isGrounded_ = false;

	// 上昇中は判定しない
	if (velocity_.y > 0.0f) { return; }

	for (const Collider* hitCol : collisionMng_.GetColliders())
	{
		if (hitCol->GetShape() != Collider::SHAPE::PLANE) continue;

		const auto* plane = static_cast<const PlaneCollider*>(hitCol);
		float height = plane->GetHeight();

		if (transform_.pos.y <= height)
		{
			transform_.pos.y = height;
			velocity_.y = 0.0f;
			isGrounded_ = true;
		}
	}
}
