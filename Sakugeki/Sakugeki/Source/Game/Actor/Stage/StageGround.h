#pragma once
#include "../../../Engine/GameObject/GameObject.h"

class CollisionManager;

// 地面（平面コライダ）
class StageGround : public GameObject
{
public:

	StageGround(CollisionManager& collisionMng);
	~StageGround(void) override;

	void Load(void) override;
	void Update(void) override;

protected:

	void InitTransform(void) override;
	void InitRenderer(void) override;
	void InitAnimation(void) override {}
	void InitPost(void) override {}

private:

	// 移動可能な半径
	static constexpr float BOUNDARY_RADIUS = 2800.0f;
};