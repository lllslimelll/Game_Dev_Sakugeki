#pragma once
#include "../Actor.h"

// 地面（平面コライダ）
class StageGround : public Actor
{
public:

	enum class COLLIDER_TYPE
	{
		PLANE,
		BOUNDARY,
		MAX
	};

	StageGround(void);
	~StageGround(void) override;

	void Load(void) override;
	void Update(void) override;

protected:

	void InitTransform(void) override;
	void InitRenderer(void) override;
	void InitCollider(void) override;
	void InitAnimation(void) override {}
	void InitPost(void) override {}

private:

	// 移動可能な半径
	static constexpr float BOUNDARY_RADIUS = 2800.0f;
};