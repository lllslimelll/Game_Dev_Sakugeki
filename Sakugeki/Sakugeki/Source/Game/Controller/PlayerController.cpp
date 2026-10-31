#include "../../Engine/Input/Input.h"
#include "../../Engine/Math/Vector2.h"
#include "../../Engine/Math/Vector3.h"
#include "../../Engine/Math/Quaternion.h"
#include "../Actor/Camera.h"
#include "../../Engine/Actor/Character/Character.h"
#include "../../Engine/Component/CharacterMovementComponent.h"
#include "PlayerController.h"

PlayerController::PlayerController(Character& character, const Camera& camera)
	:
	character_(character),
	camera_(camera)
{
}

PlayerController::~PlayerController(void)
{
}

void PlayerController::Update(void)
{
	auto& ins = Input::GetInstance();
	CharacterMovementComponent& movement = character_.GetMovement();

	// 移動入力
	Vector2 input = ins.GetMoveInput();
	if (!input.IsZero())
	{
		// カメラのY軸回転に合わせる
		Vector3 dir = camera_.GetQuaRotY().Rotate(Vector3(input.x, 0.0f, input.y));
		movement.AddMoveInput(dir.Normalized()); // 移動入力を追加
	}

	// ジャンプ
	if (ins.IsPressed(Input::Command::JUMP))
	{
		movement.Jump();
	}
}