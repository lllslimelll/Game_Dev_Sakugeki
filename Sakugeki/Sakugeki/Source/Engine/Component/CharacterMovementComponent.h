#pragma once
#include <vector>
#include "../../Engine/Math/Vector3.h"

class Transform;
class ColliderBase;
class ColliderCapsule;

/// <summary>
/// キャラクターの移動を担当するコンポーネント
/// </summary>
class CharacterMovementComponent
{
public:

	/// <summary>
	/// コンストラクタ
	/// trasnform    : 動かす対象
	/// hitColliders :衝突相手のコライダ（持ち主の Actor が管理する）
	/// </summary>
	CharacterMovementComponent(Transform& trasnform,
		const std::vector<const ColliderBase*>& hitColliders);

	// デストラクタ
	~CharacterMovementComponent();

	// 更新
	void Update();

	// 移動入力を追加
	void AddMoveInput(const Vector3& worldDir);

	// ジャンプ
	void Jump();

	// カプセルコライダを設定
	void SetCapsule(const ColliderCapsule* capsule);

	// 移動速度（単位 / 秒）
	void SetMaxWalkSpeed(float speed);

	// ジャンプの初速（単位 / 秒）
	void SetJumpSpeed(float speed);

	// 現在の速度
	const Vector3& GetVelocity() const;

	// 接地しているか
	bool IsGrounded() const;

	// 水平方向に動いているか
	bool IsMoving() const;

private:

	// 重力加速度（単位 / 秒^2）
	static constexpr float GRAVITY = 9.81f * 1.5f * 60.0f;

	// 最大落下速度（単位 / 秒）
	static constexpr float FALL_SPEED_MAX = 1800.0f;

	// 移動方向へ向きを補間する割合（1フレームあたり）
	static constexpr float ROTATE_RATE = 0.2f;

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION = 20;

	// 衝突時の押し戻し量
	static constexpr float DIS_COLLISION_BACK = 1.0f;

	// 動かす対象
	Transform& transform_;

	// 衝突相手のコライダ
	const std::vector<const ColliderBase*>& hitColliders_;

	// 押し戻しに使うカプセル
	const ColliderCapsule* capsule_;

	// パラメータ
	float maxWalkSpeed_;
	float jumpSpeed_;

	// 速度
	Vector3 velocity_;

	// このフレームの移動入力
	Vector3 moveInput_;

	// 向く方向
	Vector3 facingDir_;

	// ジャンプの指示があったか
	bool isJumpRequested_;

	// 接地しているか
	bool isGrounded_;

	// 移動方向へ向きを補間する
	void RotateToFacing(void);

	// 重力を速度に反映する
	void ApplyGravity(float deltaTime);

	// 円周の境界の内側に押し戻す
	void CollideBoundary(void);

	// モデルから押し戻す
	void CollideModel(void);

	// 地面との判定
	void CollideGround(void);
};

