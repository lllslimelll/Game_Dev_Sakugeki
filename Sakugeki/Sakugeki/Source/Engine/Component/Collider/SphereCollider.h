#pragma once
#include <DxLib.h>
#include "../../../Engine/Math/Vector3.h"
#include "Collider.h"

class Transform;

class SphereCollider : public Collider
{
public:

	// コンストラクタ
	SphereCollider(GameObject& gameObject, CollisionManager& collisionMng, TAG tag,
				   const Vector3& localPos, float radius);
	// デストラクタ
	~SphereCollider(void) override;

	// 親Transformからの相対位置を取得
	const Vector3& GetLocalPos(void) const;

	// 親Transformからの相対位置をセット
	void SetLocalPos(const Vector3& localPos);

	// ワールド座標を取得
	Vector3 GetPos(void) const;

	// 半径
	float GetRadius(void) const;
	void SetRadius(float radius);

protected:

	// デバッグ用描画
	void DrawDebug(int color) override;

private:

	// 親Transformからの相対位置（下側）
	Vector3 localPos_;

	// 半径
	float radius_;
};

