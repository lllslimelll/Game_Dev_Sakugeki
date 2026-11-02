#pragma once
#include <DxLib.h>
#include "ColliderBase.h"
class Transform;
class ColliderModel;

class ColliderLine : public ColliderBase
{
public:

	// コンストラクタ
	ColliderLine(
		TAG tag, const Transform* follow,
		const Vector3& localPosStart, const Vector3& localPosEnd);
	// デストラクタ
	~ColliderLine(void) override;

	// ローカル座標での設定
	void SetLocalPosStart(const Vector3& pos);
	void SetLocalPosEnd(const Vector3& pos);

	// ローカル座標の取得
	const Vector3& GetLocalPosStart(void) const;
	const Vector3& GetLocalPosEnd(void) const;

	// ワールド座標の取得
	Vector3 GetPosStart(void) const;
	Vector3 GetPosEnd(void) const;

	// 指定された回数と距離で三角形の法線方向に押し戻した座標を取得
	Vector3 GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const override {
		return Vector3();
	}

	// 衝突したポリゴンの上に押し戻す
	bool PushBackUp(
		const ColliderModel* colliderModel, Transform& transform,
		float pushDistance,
		bool isExclude = false, bool isTarget = false) const;

protected:

	// デバッグ用描画
	void DrawDebug(int color) override;

private:

	// デバッグ表示の球体半径
	static constexpr float RADIUS = 5.0f;
	// デバッグ表示の球体ポリゴン分割数
	static constexpr int DIV_NUM = 6;

	// 線分の開始座標(ローカル)
	Vector3 localPosStart_;
	// 線分の終了座標(ローカル)
	Vector3 localPosEnd_;
};
