#pragma once

class CollisionManager;
class DebugSystem;

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

	// デバッグ画面に Layer の名前を登録する
	void RegisterDebugNames(DebugSystem& debugSystem);
}