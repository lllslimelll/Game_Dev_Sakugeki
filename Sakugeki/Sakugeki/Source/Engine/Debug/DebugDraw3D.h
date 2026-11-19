#pragma once
#include "../Math/Vector3.h"

struct DebugSetting;

// 3D 空間（ワールド座標）に、デバッグ用の線や形を描く
class DebugDraw3D
{
public:

	// 依頼の種類（World パネルで種類ごとに表示を切り替える）
	enum class CATEGORY
	{
		GENERAL,	// 種類を指定しないもの（常に描く）
		AXES,		// 原点の軸
		GRID,		// グリッド
		COLLIDER,	// コライダー（Layer ごとに切り替える）
		RAY,		// Linecast
	};

	DebugDraw3D(void) = delete;

	// 線
	static void Line(const Vector3& start, const Vector3& end, unsigned int color,
		CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// 球（ワイヤーフレーム）
	static void Sphere(const Vector3& center, float radius, unsigned int color,
		CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// カプセル（ワイヤーフレーム）。top と bottom は両端の球の中心
	static void Capsule(const Vector3& top, const Vector3& bottom, float radius, unsigned int color,
		CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// 矢印（start から end へ。先端に円すい）
	static void Arrow(const Vector3& start, const Vector3& end, unsigned int color,
		CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// 点（XYZ 方向の小さな十字。size は中心から端までの長さ）
	static void Point(const Vector3& pos, float size, unsigned int color,
		CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// 溜めた依頼のうち、setting で表示がオンになっているものを描く
	static void Draw(const DebugSetting& setting);

	// 溜めた依頼をすべて消す
	static void Clear(void);

	// モデルのワイヤーフレーム（MeshCollider 用）
	static void ModelWireframe(int modelHandle, CATEGORY category = CATEGORY::GENERAL, int layer = 0);

	// Layer ごとのデバッグ表示の色
	static unsigned int GetLayerColor(int layer);
};