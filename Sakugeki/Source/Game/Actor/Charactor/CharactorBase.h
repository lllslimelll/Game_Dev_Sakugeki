#pragma once
#include <memory>
#include "../ActorBase.h"

class AnimationController;

class CharactorBase : public ActorBase
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
	CharactorBase(void);

	// デストラクタ
	virtual ~CharactorBase(void);

	// 更新
	virtual void Update(void) override;
	// 解放
	void Release() override;

protected:

	// アニメーション
	std::unique_ptr <AnimationController> animController_;

	// 移動方向
	VECTOR moveDir_;

	// 移動スピード
	float moveSpeed_;

	// 移動量
	VECTOR movePow_;

	// ジャンプ量
	VECTOR jumpPow_;

	// 移動前の座標
	VECTOR prevPos_;

	// ジャンプ判定
	bool isJump_;

	// 重力
	static constexpr float GRAVITY = 9.81f;
	static constexpr float GRAVITY_SCALE = 1.5f;

	// 最大落下速度
	static constexpr float FALL_SPEED_MAX = -30.0f;

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION = 20;
	// 衝突時の押し戻し量
	static constexpr float DIS_COLLISION_BACK = 1.0f;

	// 更新系
	virtual void UpdateProcess(void) = 0;

	// 移動方向に応じた遅延回転
	void DelayRotate(void);

	// 重力計算
	void CalcGravityPow(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCircleBoundary(void);
	void CollisionCapsule(void);
};

