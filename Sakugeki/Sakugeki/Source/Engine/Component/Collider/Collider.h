#pragma once
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Component/Component.h"

class Transform;
class CollisionManager;

// 当たり判定の基底コンポーネント
class Collider : public Component
{
public:

	// 形状
	enum class SHAPE
	{
		NONE,
		SPHERE,
		CAPSULE,
		MESH,
		CIRCLE_BOUNDARY,
	};

	// コンストラクタ
	Collider(GameObject& gameObject, CollisionManager& collisionMng, 
			 SHAPE shape, int layer);
	// デストラクタ
	virtual ~Collider();

	// 初期化
	void Init() override;

	// 解放
	void Release() override;

	// 追従先の取得
	const Transform& GetTransform() const;

	// 形状
	SHAPE GetShape(void) const;

	// 衝突種別
	int GetLayer() const;

	// 自分の形を DebugDraw3D に渡す
	virtual void AddDebugShape(unsigned int color) const = 0;

protected:

	// ローカル座標をワールド座標に変換
	Vector3 GetRotPos(const Vector3& localPos) const;

private:

	// 当たり判定の管理クラス
	CollisionManager& collisionMng_;

	// 形状
	SHAPE shape_;

	// 衝突種別
	int layer_;
};
