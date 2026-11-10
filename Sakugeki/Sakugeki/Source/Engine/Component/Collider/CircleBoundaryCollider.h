#pragma once
#include <DxLib.h>
#include "../../../Engine/Common/Transform.h"
#include "../../../Engine/Component/Collider/Collider.h"

// 円周の境界コライダ
class CircleBoundaryCollider : public Collider
{
public:

	CircleBoundaryCollider(
		GameObject& gameObject, CollisionManager& collisionMng, int layer, float radius);
	~CircleBoundaryCollider(void) override;

	// 制限半径
	float GetRadius(void) const;

protected:

	// デバッグ表示
	void DrawDebug(int color) override {}

private:

	// 制限半径
	float radius_;
};