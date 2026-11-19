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

	void AddDebugShape(unsigned int color) const override;

private:

	// 制限半径
	float radius_;
};