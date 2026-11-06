#pragma once
#include "../Math/Vector3.h"
#include "Component.h"

class CharacterController;

// キャラクターの移動の値（速度・重力・ジャンプ・向き）を担当するコンポーネントクラス
// 位置の更新と衝突は CharacterController に任せる
class CharacterMover : public Component
{
public:

	// コンストラクタ
	explicit CharacterMover(GameObject& gameObject);

	// デストラクタ
	~CharacterMover(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 移動入力を追加
	void AddMoveInput(const Vector3& worldDir);

	// ジャンプ
	void Jump(void);

	// 移動速度（単位 / 秒）
	void SetMaxWalkSpeed(float speed);

	// ジャンプの初速（単位 / 秒）
	void SetJumpSpeed(float speed);

	// 現在の速度
	const Vector3& GetVelocity(void) const;

	// 接地しているか（CharacterController の結果を返す）
	bool IsGrounded(void) const;

	// 水平方向に動いているか
	bool IsMoving(void) const;

private:

	// 重力加速度（単位 / 秒^2）
	static constexpr float GRAVITY = 9.81f * 1.5f * 60.0f;

	// 最大落下速度（単位 / 秒）
	static constexpr float FALL_SPEED_MAX = 1800.0f;

	// 移動方向へ向きを補間する割合（1フレームあたり）
	static constexpr float ROTATE_RATE = 0.2f;

	// 位置を動かすコンポーネント
	CharacterController* characterController_;

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

	// 移動方向へ向きを補間する
	void RotateToFacing(void);

	// 重力を速度に反映する
	void ApplyGravity(float deltaTime);
};