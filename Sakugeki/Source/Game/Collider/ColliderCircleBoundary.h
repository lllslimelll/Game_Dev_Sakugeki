#pragma once
#include <DxLib.h>
#include "../../Engine/Common/Transform.h"
#include "ColliderBase.h"

// 円周の境界コライダ
class ColliderCircleBoundary : public ColliderBase
{
public:

	ColliderCircleBoundary(TAG tag, const Transform* follow, float radius);
	~ColliderCircleBoundary(void) override;

	// 制限半径
	float GetRadius(void) const;

	// pos を円の内側に押し戻す（XZ 平面のみ）。押し戻したら true
	// bodyRadius: 押し戻す側の体の半径
	bool PushBackInside(VECTOR& pos, float bodyRadius) const;

	// このクラスでは実装しない純粋仮想関数
	VECTOR GetPosPushBackAlongNormal(
		const MV1_COLL_RESULT_POLY& hitPoly,
		int maxTryCnt,
		float pushDistance) const override { return VECTOR();}

protected:

	// デバッグ表示
	void DrawDebug(int color) override {}

private:

	// 制限半径
	float radius_;
};