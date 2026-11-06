#pragma once
#include "../../Math/Vector3.h"

class Collider;

// Õ“Ëî•ñ
struct ContactPoint
{
	Vector3 normal = Vector3::ZERO;		// ‰Ÿ‚µ–ß‚·•ûŒü
	float depth = 0.0f;					// ‚ß‚è‚İ—Ê
	const Collider* other = nullptr;	// ‘Šè
};