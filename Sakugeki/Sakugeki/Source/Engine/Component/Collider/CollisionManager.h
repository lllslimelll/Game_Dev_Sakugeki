#pragma once
#include <vector>

class Collider;
struct ContactPoint;

// 当たり判定の管理クラス
class CollisionManager
{
public:

	// コンストラクタ
	CollisionManager(void);

	// デストラクタ
	~CollisionManager(void);

	// コピー禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// コライダーを登録
	void Register(const Collider* collider);

	// コライダーの登録を解除
	void Unregister(const Collider* collider);

	// コライダーを取得
	const std::vector<const Collider*>& GetColliders(void) const;

	// 最小の押し戻し方向と距離を計算
	// a が b にめり込んでいれば true を返し、
	// a を b から押し戻す方向と距離を outContactPoint に入れる
	bool ComputePenetration(const Collider& a, const Collider& b, ContactPoint& outContactPoint) const;

private:

	// コライダー
	std::vector<const Collider*> colliders_;
};