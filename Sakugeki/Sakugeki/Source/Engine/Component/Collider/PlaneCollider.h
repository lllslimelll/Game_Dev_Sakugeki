#pragma once
#include "Collider.h"

class PlaneCollider : public Collider
{
public:

    PlaneCollider(GameObject& gameObject, CollisionManager& collisionMng, int layer);

    // 平面（地面）の高さ
    const float GetHeight(void) const;

protected:

    // このクラスでは実装しない純粋仮想関数
    void DrawDebug(int color) override {};
};

