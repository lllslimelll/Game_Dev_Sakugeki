#pragma once
#include "../Math/Vector3.h"

// 3D 空間（ワールド座標）に、デバッグ用の線や形を描く
class DebugDraw3D
{
public:

	DebugDraw3D(void) = delete;

	// 線
	static void Line(const Vector3& start, const Vector3& end, unsigned int color);

	// 球（ワイヤーフレーム）
	static void Sphere(const Vector3& center, float radius, unsigned int color);

	// カプセル（ワイヤーフレーム）。top と bottom は両端の球の中心
	static void Capsule(const Vector3& top, const Vector3& bottom, float radius, unsigned int color);

	// 矢印（start から end へ。先端に円すい）
	static void Arrow(const Vector3& start, const Vector3& end, unsigned int color);

	// 点（XYZ 方向の小さな十字。size は中心から端までの長さ）
	static void Point(const Vector3& pos, float size, unsigned int color);

	// 溜めた依頼をすべて描く
	static void Draw(void);

	// 溜めた依頼をすべて消す
	static void Clear(void);
};