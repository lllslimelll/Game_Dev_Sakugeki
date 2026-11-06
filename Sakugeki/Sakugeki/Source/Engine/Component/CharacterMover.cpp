#include <algorithm>
#include <cassert>
#include "../../Engine/Common/Transform.h"
#include "../../Engine/Math/Quaternion.h"
#include "../../Engine/Scene/SceneManager.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../../Engine/Component/CharacterController.h"
#include "../../Engine/Component/CharacterMover.h"

CharacterMover::CharacterMover(GameObject& gameObject)
	:
	Component(gameObject),
	characterController_(nullptr),
	maxWalkSpeed_(0.0f),
	jumpSpeed_(0.0f),
	velocity_(Vector3::ZERO),
	moveInput_(Vector3::ZERO),
	facingDir_(Vector3::ZERO),
	isJumpRequested_(false)
{
}

CharacterMover::~CharacterMover(void)
{
}

void CharacterMover::Init(void)
{
	characterController_ = gameObject_.GetComponent<CharacterController>();
	assert(controller_ != nullptr && "CharacterMover には CharacterController が必要です");
}

void CharacterMover::Update(void)
{
	const float deltaTime = SceneManager::GetInstance().GetDeltaTime();

	// 水平方向の速度
	Vector3 input(moveInput_.x, 0.0f, moveInput_.z);
	if (input.SquareMagnitude() > 1.0f) 
	{
		input = input.Normalized();
	}
	velocity_.x = input.x * maxWalkSpeed_;
	velocity_.z = input.z * maxWalkSpeed_;

	// 入力があれば、その方向を向く
	if (!input.IsZero())
	{
		facingDir_ = input.Normalized();
	}

	// ジャンプ
	if (isJumpRequested_ && characterController_->IsGrounded())
	{
		velocity_.y = jumpSpeed_;
	}

	// 移動方向へ向きを補間
	RotateToFacing();

	// 重力
	ApplyGravity(deltaTime);

	// 移動（位置の更新と衝突は CharacterController に任せる）
	const CharacterController::MoveResult result = characterController_->Move(velocity_ * deltaTime);

	// 着地したら落下を止める
	if (result.isBelow)
	{
		velocity_.y = 0.0f;
	}

	// 次のフレームのために入力をリセット
	moveInput_ = Vector3::ZERO;
	isJumpRequested_ = false;
}

void CharacterMover::AddMoveInput(const Vector3& worldDir)
{
	moveInput_ += worldDir;
}

void CharacterMover::Jump(void)
{
	isJumpRequested_ = true;
}

void CharacterMover::SetMaxWalkSpeed(float speed)
{
	maxWalkSpeed_ = speed;
}

void CharacterMover::SetJumpSpeed(float speed)
{
	jumpSpeed_ = speed;
}

const Vector3& CharacterMover::GetVelocity(void) const
{
	return velocity_;
}

bool CharacterMover::IsGrounded(void) const
{
	return characterController_->IsGrounded();
}

bool CharacterMover::IsMoving(void) const
{
	return !Vector3(velocity_.x, 0.0f, velocity_.z).IsZero();
}

void CharacterMover::RotateToFacing(void)
{
	if (facingDir_.IsZero()) { return; }

	Transform& transform = gameObject_.GetTransform();

	// 移動方向から回転に変換する
	Quaternion goalRot = Quaternion::LookRotation(facingDir_);

	// 回転の補間
	transform.quaRot = Quaternion::Slerp(transform.quaRot, goalRot, ROTATE_RATE);
}

void CharacterMover::ApplyGravity(float deltaTime)
{
	velocity_.y -= GRAVITY * deltaTime;

	// 落下速度の上限
	velocity_.y = (std::max)(velocity_.y, -FALL_SPEED_MAX);
}