#include "../../../Engine/Common/AnimationController.h"
#include "../../Component/CharacterMovementComponent.h"
#include "Character.h"

Character::Character(void)
	:
	GameObject(),
	movementComp_(nullptr),
	animController_()
{
	movementComp_ = AddComponent<CharacterMovementComponent>(transform_, hitColliders_);
}

Character::~Character(void)
{
}

void Character::Update(void)
{
	// コンポーネントの更新
	GameObject::Update();

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

	GameObject::Release();
}

CharacterMovementComponent& Character::GetMovement(void)
{
	return *movementComp_;
}

const CharacterMovementComponent& Character::GetMovement(void) const
{
	return *movementComp_;
}
