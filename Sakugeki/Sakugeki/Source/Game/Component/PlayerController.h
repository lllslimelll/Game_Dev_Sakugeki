#pragma once
#include "../../Engine/Component/Component.h"

class Camera;
class Animator;
class CharacterMover;

// プレイヤーの操作を行うコンポーネントクラス
class PlayerController : public Component
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
	PlayerController(GameObject& gameObject, const Camera& camera);

	// デストラクタ
	~PlayerController(void) override; 

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;  

private:

	// カメラ
	const Camera& camera_;

	// 移動用コンポーネント
	CharacterMover* movementComp_;

	// アニメーション
	Animator* animatorComp_;

	// 移動の更新処理
	void UpdateMove(void);

	// アニメーション更新処理
	void UpdateAnimation(void);
};