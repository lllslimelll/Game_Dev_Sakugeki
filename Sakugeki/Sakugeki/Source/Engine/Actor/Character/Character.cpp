#include "../../Component/CharacterMovementComponent.h"
#include "Character.h"

Character::Character(void)
	:
	GameObject(),
	movementComp_(nullptr)
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

	// 移動の状態からアニメーションを選ぶ
	UpdateAnimation();
}

void Character::Release()
{
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
