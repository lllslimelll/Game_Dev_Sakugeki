#pragma once
#include "../../Math/Vector3.h"

class Collider;

// 衝突情報
struct ContactPoint
{
	Vector3 point = Vector3::ZERO;		// 衝突点
	Vector3 normal = Vector3::ZERO;		// 法線（押し戻す方向）
	float depth = 0.0f;					// めり込み量
	const Collider* other = nullptr;	// 衝突相手のコライダー
};