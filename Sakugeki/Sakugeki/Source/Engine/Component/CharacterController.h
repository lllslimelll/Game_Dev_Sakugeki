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
		bool isBelow = false;	// 床に当たったか（接地）
		bool isSides = false;	// 押し戻されたか
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

	// めり込んでいるコライダーから押し出す（押し戻したら true）
	bool ResolveCollisions(void);

	// 地面との判定（接地したら true）
	bool CollideGround(float prevY, bool isRising);

	// 法線が立てる床の向きか 
	static bool IsWalkable(const Vector3& normal);

	// 接地判定のレイ
	// 足元のこの高さからレイを始める（段差を上れる高さの目安）
	static constexpr float GROUND_RAY_UP = 30.0f;
	// 前のフレームで接地していたら、この距離だけ下まで吸い付く（下り坂で浮かないように）
	static constexpr float GROUND_SNAP = 20.0f;
	// 立てる坂の限界角度
	static constexpr float SLOPE_LIMIT_DEG = 45.0f;

	// 当たり判定の管理クラス
	const CollisionManager& collisionMng_;

	// 押し戻しに使うカプセル
	const CapsuleCollider* capsule_;

	// 接地しているか
	bool isGrounded_;

	// 押し戻しを繰り返す最大回数
	static constexpr int MAX_RESOLVE_ITERATIONS = 4;
};