#include <DxLib.h>
#include "ColliderBase.h"
class Transform;
class ColliderModel;

class ColliderCapsule : public ColliderBase
{
public:

	// コンストラクタ
	ColliderCapsule(TAG tag, const Transform* follow,
		const Vector3& localPosTop, const Vector3& localPosDown, float radius);

	// デストラクタ
	~ColliderCapsule(void);

	// 親Transformからの相対位置を取得
	const Vector3& GetLocalPosTop(void) const;
	const Vector3& GetLocalPosDown(void) const;

	// 親Transformからの相対位置をセット
	void SetLocalPosTop(const Vector3& pos);
	void SetLocalPosDown(const Vector3& pos);

	// ワールド座標を取得
	Vector3 GetPosTop(void) const;
	Vector3 GetPosDown(void) const;

	// 半径
	float GetRadius(void) const;
	void SetRadius(float radius);

	// 高さ
	float GetHeight(void) const;

	// カプセルの中心座標
	Vector3 GetCenter(void) const;

	// 指定された回数と距離で三角形の法線方向に押し戻す
	void PushBackAlongNormal(
		const ColliderModel* colliderModel, Transform& transform,
		int maxTryCnt, float pushDistance,
		bool isExclude = false, bool isTarget = false) const;

	// 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	Vector3 GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const override;

	// 衝突してるか否か（対モデル）
	bool IsHit(const ColliderModel* colliderModel,
		bool isExclude = false, bool isTarget = false) const;

protected:

	// デバッグ用描画
	void DrawDebug(int color) override;

private:

	// 親Transformからの相対位置(上端)
	Vector3 localPosTop_;
	// 親Transformからの相対位置(下端)
	Vector3 localPosDown_;

	// 半径
	float radius_;
};

