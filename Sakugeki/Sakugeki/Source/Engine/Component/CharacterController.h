#pragma once
#include "../Math/Vector3.h"
#include "Component.h"

class CapsuleCollider;
class CollisionManager;

// 衝突を考慮しながらキャラクターを動かすコンポーネントクラス
class CharacterController : public Component
{
public:

	// Move の結果
	struct MoveResult
	{
		bool isBelow = false;	// 床に当たった（接地）
		bool isSides = false;	// 押し戻された
	};

	/// コンストラクタ
	/// collisionMng : 当たり判定の管理クラス
	CharacterController(GameObject& gameObject, const CollisionManager& collisionMng);

	// デストラクタ
	~CharacterController(void) override;

	// 初期化
	void Init(void) override;

	// 移動量 delta だけ動かし、衝突を解決する
	MoveResult Move(const Vector3& delta);

	// 接地しているか
	bool IsGrounded(void) const;

private:

	// 当たり判定の管理クラス
	const CollisionManager& collisionMng_;

	// 押し戻しに使うカプセル
	const CapsuleCollider* capsule_;

	// 接地しているか
	bool isGrounded_;

	// 押し戻しを繰り返す最大回数
	static constexpr int MAX_RESOLVE_ITERATIONS = 4;

	// めり込んでいるコライダーから押し出す（押し戻したら true）
	bool ResolveCollisions(void);

	// 地面との判定（接地したら true）
	bool CollideGround(bool isRising);

	// 地面との判定（接地したら true）
	bool CollideGround(bool isRising);
};