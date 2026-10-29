#pragma once
#include <DxLib.h>
#include "../../Engine/Math/Vector3.h"
#include "ColliderBase.h"

class Transform;

class ColliderSphere : public ColliderBase
{
public:

	// コンストラクタ
	ColliderSphere(
		TAG tag, const Transform* follow, const Vector3& localPos, float radius);
	// デストラクタ
	~ColliderSphere(void);

	// 親Transformからの相対位置を取得
	const Vector3& GetLocalPos(void) const;

	// 親Transformからの相対位置をセット
	void SetLocalPos(const Vector3& localPos);

	// ワールド座標を取得
	Vector3 GetPos(void) const;

	// 半径
	float GetRadius(void) const;
	void SetRadius(float radius);

	// 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	Vector3 GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const override;

protected:

	// デバッグ用描画
	void DrawDebug(int color) override;

private:

	// 親Transformからの相対位置（下側）
	Vector3 localPos_;

	// 半径
	float radius_;
};

