#pragma once
#include "../../Engine/Component/Component.h"

class Camera;
class AnimatorComponent;

// プレイヤーの操作を行うクラス
class PlayerControllerComponent : public Component
{
public:

	// アニメーションの種類
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		JUMP,
	};

	/// <summary>
	/// コンストラクタ
	/// gameObject : 持ち主の GameObject
	/// camera    : 移動方向の基準にするカメラ
	/// </summary>
	PlayerControllerComponent(GameObject& gameObject, const Camera& camera);

	// デストラクタ
	~PlayerControllerComponent(void);

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void);

private:

	// カメラ
	const Camera& camera_;

	// 移動用コンポーネント
	CharacterMovementComponent* movementComp_;

	// アニメーション
	AnimatorComponent* animatorComp_;

	// 移動の更新処理
	void UpdateMove(void);

	// アニメーション更新処理
	void UpdateAnimation(void);
};
