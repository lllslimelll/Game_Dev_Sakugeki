#pragma once
#include "../../../Engine/GameObject/GameObject.h"

class CollisionManager;

// 地面（平面コライダ）
class Ground : public GameObject
{
public:

	Ground(CollisionManager& collisionMng);
	~Ground(void) override;

	void Load(void) override;
	void Update(void) override;

protected:

	void InitTransform(void) override;
	void InitRenderer(void) override;
	void InitAnimation(void) override {}
	void InitPost(void) override {}

private:

};