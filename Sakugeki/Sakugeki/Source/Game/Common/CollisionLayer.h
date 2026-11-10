#pragma once

class CollisionManager;

// 衝突レイヤー
namespace CollisionLayer
{
	enum : int
	{
		GROUND,
		WALL,
		TREE,
		PLAYER,
		ENEMY,
		CAMERA,
	};

	// 衝突表を設定する
	void SetupMatrix(CollisionManager& collisionMng);
}