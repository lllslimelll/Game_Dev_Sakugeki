#pragma once
#include <DxLib.h>
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
		LINE,
		SPHERE,
		CAPSULE,
		MODEL,
		PLANE,
		CIRCLE_BOUNDARY,
	};

	// 衝突種別
	enum class TAG
	{
		GROUND,
		WALL,
		PLAYER,
		CAMERA,
		ENEMY,
	};

	// コンストラクタ
	Collider(GameObject& gameObject, CollisionManager& collisionMng, 
			 SHAPE shape, TAG tag);
	// デストラクタ
	virtual ~Collider();

	// 初期化
	void Init() override;

	// 描画
	void Draw() override;

	// 解放
	void Release() override;

	// 追従先の取得
	const Transform& GetTransform() const;

	// 形状
	SHAPE GetShape(void) const { return shape_; }

	// 衝突種別
	TAG GetTag(void) const { return tag_; }

	// 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	virtual Vector3 GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const = 0;

protected:

	// デバッグ表示の色
	static constexpr int COLOR_DEBUG = 0xff0000;

	// ローカル座標をワールド座標に変換
	Vector3 GetRotPos(const Vector3& localPos) const;

	// デバッグ用描画
	virtual void DrawDebug(int color) = 0;

private:

	// 当たり判定の管理クラス
	CollisionManager& collisionMng_;

	// 形状
	SHAPE shape_;

	// 衝突種別
	TAG tag_;
};
