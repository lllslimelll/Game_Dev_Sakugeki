#include "../../Engine/Input/Input.h"
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Quaternion.h"
#include "../../Engine/GameObject/GameObject.h"
#include "../../Engine/Component/CharacterMovement.h"
#include "../../Engine/Component/Animator.h"
#include "../Actor/Camera.h"
#include "PlayerControllerComponent.h"

PlayerControllerComponent::PlayerControllerComponent(GameObject& gameObject, const Camera& camera)
	:
	Component(gameObject),
	camera_(camera),
	movementComp_(nullptr),
	animatorComp_(nullptr)
{
}

PlayerControllerComponent::~PlayerControllerComponent(void)
{
}

void PlayerControllerComponent::Init(void)
{
	// コンポーネントの取得
	movementComp_ = gameObject_.GetComponent<CharacterMovement>();
	animatorComp_ = gameObject_.GetComponent<Animator>();
}

void PlayerControllerComponent::Update(void)
{
	UpdateMove();
	UpdateAnimation();
}

void PlayerControllerComponent::UpdateMove(void)
{
	auto& ins = Input::GetInstance();

	// 移動入力
	Vector2 input = ins.GetMoveInput();
	if (!input.IsZero())
	{
		// カメラのY軸回転に合わせる
		Vector3 dir = camera_.GetQuaRotY().Rotate(Vector3(input.x, 0.0f, input.y));
		movementComp_->AddMoveInput(dir.Normalized()); // 移動入力を追加
	}

	// ジャンプ
	if (ins.IsPressed(Input::Command::JUMP))
	{
		movementComp_->Jump();
	}
}

void PlayerControllerComponent::UpdateAnimation(void)
{
	// 移動の状態から選ぶ
	ANIM_TYPE type = ANIM_TYPE::IDLE;
	if (!movementComp_->IsGrounded())
	{
		type = ANIM_TYPE::JUMP;
	}
	else if (movementComp_->IsMoving())
	{
		type = ANIM_TYPE::RUN;
	}

	// ジャンプだけはループしない
	bool isLoop = (type != ANIM_TYPE::JUMP);
	animatorComp_->Play(static_cast<int>(type), isLoop);
}
