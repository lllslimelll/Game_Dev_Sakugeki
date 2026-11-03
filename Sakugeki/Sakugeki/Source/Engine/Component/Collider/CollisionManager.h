#pragma once
#include <vector>

class ColliderBase;

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
	void Register(const ColliderBase* collider);

	// コライダーの登録を解除
	void Unregister(const ColliderBase* collider);

	// コライダーを取得
	const std::vector<const ColliderBase*>& GetColliders(void) const;

private:

	// コライダー
	std::vector<const ColliderBase*> colliders_;
};