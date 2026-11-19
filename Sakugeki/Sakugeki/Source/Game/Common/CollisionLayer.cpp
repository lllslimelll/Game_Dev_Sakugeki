#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Engine/Debug/DebugSystem.h"
#include "../../Game/Common/CollisionLayer.h"

void CollisionLayer::SetupMatrix(CollisionManager& collisionMng)
{
	// 初期状態はすべて当たる設定なので、当たらない組み合わせだけを切る

	// カメラ
	//collisionMng.SetLayerCollision(CAMERA, TREE, false);
	collisionMng.SetLayerCollision(CAMERA, PLAYER, false);
	collisionMng.SetLayerCollision(CAMERA, ENEMY, false);
}

void CollisionLayer::RegisterDebugNames(DebugSystem& debugSystem)
{
	debugSystem.SetLayerName(GROUND, "GROUND");
	debugSystem.SetLayerName(WALL, "WALL");
	debugSystem.SetLayerName(TREE, "TREE");
	debugSystem.SetLayerName(PLAYER, "PLAYER");
	debugSystem.SetLayerName(ENEMY, "ENEMY");
	debugSystem.SetLayerName(CAMERA, "CAMERA");
}
