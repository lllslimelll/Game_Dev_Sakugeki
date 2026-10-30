#pragma once
#include <memory>
#include "../Actor.h"

class AnimationController;
class CharacterMovementComponent;

class Character : public Actor
{
public:

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		GROUND_LINE,
		CAPSULE,
		MAX,
	};

	// コンストラクタ
	Character(void);

	// デストラクタ
	virtual ~Character(void);

	// 更新
	virtual void Update(void) override;
	// 解放
	void Release() override;

	// 移動コンポーネント
	CharacterMovementComponent& GetMovement();
	const CharacterMovementComponent& GetMovement() const;

protected:

	// 移動
	std::unique_ptr<CharacterMovementComponent> movement_;

	// アニメーション
	std::unique_ptr <AnimationController> animController_;

	// 移動の状態から再生するアニメーションを選ぶ
	virtual void UpdateAnimation() {};
};

