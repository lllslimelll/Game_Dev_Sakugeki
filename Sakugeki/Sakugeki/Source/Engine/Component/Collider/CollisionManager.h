#pragma once
#include <vector>

class Collider;

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

private:

	// コライダー
	std::vector<const Collider*> colliders_;
};