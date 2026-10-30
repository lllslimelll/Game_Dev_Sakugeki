#include "../../../Engine/Common/AnimationController.h"
#include "../../Component/CharacterMovementComponent.h"
#include "Character.h"

Character::Character(void)
	:
	Actor(),
	movement_(std::make_unique<CharacterMovementComponent>(transform_, hitColliders_)),
	animController_()
{
}

Character::~Character(void)
{
}

void Character::Update(void)
{
	// 移動
	movement_->Update();

	// モデル制御更新
	transform_.Update();

	// 移動の状態からアニメーションを選ぶ
	UpdateAnimation();

	// アニメーション再生
	if (animController_ != nullptr)
	{
		animController_->Update();
	}
}

void Character::Release()
{
	// アニメーションのリソース解放
	if (animController_ != nullptr)
	{
		animController_->Release();
	}

	Actor::Release();
}

CharacterMovementComponent& Character::GetMovement(void)
{
	return *movement_;
}

const CharacterMovementComponent& Character::GetMovement(void) const
{
	return *movement_;
}
