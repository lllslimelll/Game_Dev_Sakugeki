#pragma once
#include "../Math/Vector3.h"
#include "Component.h"

class CapsuleCollider;
class CollisionManager;

// 衝突を解決しながらキャラクターを動かすコンポーネントクラス
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

	// 衝突時の押し戻し試行回数
	static constexpr int CNT_TRY_COLLISION = 20;

	// 衝突時の押し戻し量
	static constexpr float DIS_COLLISION_BACK = 1.0f;

	// 当たり判定の管理クラス
	const CollisionManager& collisionMng_;

	// 押し戻しに使うカプセル
	const CapsuleCollider* capsule_;

	// 接地しているか
	bool isGrounded_;

	// 円周の境界の内側に押し戻す（押し戻したら true）
	bool CollideBoundary(void);

	// メッシュから押し戻す（押し戻したら true）
	bool CollideModel(void);

	// 地面との判定（接地したら true）
	bool CollideGround(bool isRising);
};