#pragma once
#include "../../Math/Vector3.h"

class Collider;

// レイの衝突情報
struct RaycastHit
{
	Vector3 point = Vector3::ZERO;		// 衝突点
	Vector3 normal = Vector3::ZERO;		// 衝突面の法線
	float distance = 0.0f;				// レイの始点から衝突点までの距離
	const Collider* other = nullptr;	// 衝突相手のコライダー
};