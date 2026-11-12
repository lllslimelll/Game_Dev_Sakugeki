#include "../../Engine/Component/Collider/CollisionManager.h"
#include "../../Game/Common/CollisionLayer.h"

void CollisionLayer::SetupMatrix(CollisionManager& collisionMng)
{
	// 初期状態はすべて当たる設定なので、当たらない組み合わせだけを切る

	// カメラ
	//collisionMng.SetLayerCollision(CAMERA, TREE, false);
	collisionMng.SetLayerCollision(CAMERA, PLAYER, false);
	collisionMng.SetLayerCollision(CAMERA, ENEMY, false);
}