#pragma once
#include "../../Engine/Common/Transform.h"
#include "ColliderBase.h"

class ColliderPlane : public ColliderBase
{
public:

    ColliderPlane(TAG tag, const Transform* follow);

    // 平面（地面）の高さ
    const float GetHeight(void) const;

    // このクラスでは実装しない純粋仮想関数
    Vector3 GetPosPushBackAlongNormal(
        const MV1_COLL_RESULT_POLY& hitPoly,
        int maxTryCnt,
        float pushDistance) const override { return Vector3(); }

protected:

    // このクラスでは実装しない純粋仮想関数
    void DrawDebug(int color) override {};
};

