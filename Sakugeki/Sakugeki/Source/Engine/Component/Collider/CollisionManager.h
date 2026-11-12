#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <cstdint>

class GameObject;
class Vector3;
class Collider;
struct ContactPoint;
struct RaycastHit;


// 当たり判定の管理クラス
class CollisionManager
{
public:

	// Layer の集まり
	using LayerMask = std::uint32_t;

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
	bool ComputePenetration(const Collider& a, const Collider& b, ContactPoint& contactPoint) const;

	// start から end までの線分で、mask に含まれる Layer のコライダーを調べ、hitResult に入れる
	// ignore を指定すると、その GameObject のコライダーは無視する
	bool Linecast(const Vector3& start, const Vector3& end, LayerMask layerMask,
		RaycastHit& hitResult, const GameObject* ignore = nullptr) const;

	// start から dir の向きに maxDistance まで調べる
	bool Raycast(const Vector3& start, const Vector3& dir, float maxDistance, LayerMask layerMask,
		RaycastHit& hitResult, const GameObject* ignore = nullptr) const;

	// layer が衝突表で当たる相手の Layer の集まり
	LayerMask GetCollisionMask(int layer) const;

	// レイヤー a と レイヤー b が当たるかを設定する
	void SetLayerCollision(int a, int b, bool isCollide);

	// レイヤー a と レイヤー b が当たるか
	bool IsLayerCollide(int a, int b) const;

	// 2つのコライダーが当たり判定の対象か
	bool CanCollide(const Collider& a, const Collider& b) const;

	// すべての Layer
	static constexpr LayerMask ALL_LAYERS = 0xFFFFFFFFu;

	// レイヤーの最大数
	static constexpr int MAX_LAYERS = 32;

private:

	// コライダー
	std::vector<const Collider*> colliders_;

	// 衝突表
	std::array<std::uint32_t, MAX_LAYERS> layerMasks_;
};