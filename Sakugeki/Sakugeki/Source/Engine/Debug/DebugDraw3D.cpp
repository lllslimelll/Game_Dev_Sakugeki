#include <vector>
#include <DxLib.h>
#include "DebugConfig.h"
#include "DebugDraw3D.h"

namespace
{
	// 形の種類
	enum class SHAPE
	{
		LINE,
		SPHERE,
		CAPSULE,
		ARROW,
		POINT,
	};

	// 描く依頼1つ分
	// a, b の意味は形ごとに違う（線と矢印は始点と終点、球と点は中心、カプセルは両端）
	struct Shape
	{
		SHAPE type = SHAPE::LINE;
		Vector3 a;
		Vector3 b;
		float size = 0.0f;			// 球とカプセルは半径、点は十字の大きさ
		unsigned int color = 0;
	};

	// 溜めた依頼
	std::vector<Shape> shapes;

	// 球やカプセルの細かさ（大きいほど丸いが重い）
	constexpr int SPHERE_DIV = 12;

	// 矢印の先端
	constexpr float ARROW_HEAD_RATE = 0.2f;		// 全体の長さに対する先端の長さ
	constexpr float ARROW_HEAD_WIDTH = 0.35f;	// 先端の長さに対する根元の半径
	constexpr int ARROW_HEAD_DIV = 8;

	void Add(SHAPE type, const Vector3& a, const Vector3& b, float size, unsigned int color)
	{
		Shape shape;
		shape.type = type;
		shape.a = a;
		shape.b = b;
		shape.size = size;
		shape.color = color;
		shapes.push_back(shape);
	}

	void DrawArrow(const Shape& shape)
	{
		DrawLine3D(shape.a.ToVECTOR(), shape.b.ToVECTOR(), shape.color);

		const Vector3 diff = shape.b - shape.a;
		if (diff.IsZero()) { return; }

		// 先端の円すい（頂点が終点、底面が少し手前）
		const float headLength = diff.Magnitude() * ARROW_HEAD_RATE;
		const Vector3 headBottom = shape.b - diff.Normalized() * headLength;
		DrawCone3D(shape.b.ToVECTOR(), headBottom.ToVECTOR(), headLength * ARROW_HEAD_WIDTH,
			ARROW_HEAD_DIV, shape.color, shape.color, FALSE);
	}

	void DrawPoint(const Shape& shape)
	{
		const Vector3& p = shape.a;
		const float s = shape.size;
		DrawLine3D((p + Vector3(-s, 0.0f, 0.0f)).ToVECTOR(), (p + Vector3(s, 0.0f, 0.0f)).ToVECTOR(), shape.color);
		DrawLine3D((p + Vector3(0.0f, -s, 0.0f)).ToVECTOR(), (p + Vector3(0.0f, s, 0.0f)).ToVECTOR(), shape.color);
		DrawLine3D((p + Vector3(0.0f, 0.0f, -s)).ToVECTOR(), (p + Vector3(0.0f, 0.0f, s)).ToVECTOR(), shape.color);
	}
}

void DebugDraw3D::Line(const Vector3& start, const Vector3& end, unsigned int color)
{
#if USE_DEBUG_TOOLS
	Add(SHAPE::LINE, start, end, 0.0f, color);
#endif
}

void DebugDraw3D::Sphere(const Vector3& center, float radius, unsigned int color)
{
#if USE_DEBUG_TOOLS
	Add(SHAPE::SPHERE, center, center, radius, color);
#endif
}

void DebugDraw3D::Capsule(const Vector3& top, const Vector3& bottom, float radius, unsigned int color)
{
#if USE_DEBUG_TOOLS
	Add(SHAPE::CAPSULE, top, bottom, radius, color);
#endif
}

void DebugDraw3D::Arrow(const Vector3& start, const Vector3& end, unsigned int color)
{
#if USE_DEBUG_TOOLS
	Add(SHAPE::ARROW, start, end, 0.0f, color);
#endif
}

void DebugDraw3D::Point(const Vector3& pos, float size, unsigned int color)
{
#if USE_DEBUG_TOOLS
	Add(SHAPE::POINT, pos, pos, size, color);
#endif
}

void DebugDraw3D::Draw(void)
{
#if USE_DEBUG_TOOLS
	for (const Shape& shape : shapes)
	{
		switch (shape.type)
		{
		case SHAPE::LINE:
			DrawLine3D(shape.a.ToVECTOR(), shape.b.ToVECTOR(), shape.color);
			break;
		case SHAPE::SPHERE:
			DrawSphere3D(shape.a.ToVECTOR(), shape.size, SPHERE_DIV, shape.color, shape.color, FALSE);
			break;
		case SHAPE::CAPSULE:
			DrawCapsule3D(shape.a.ToVECTOR(), shape.b.ToVECTOR(), shape.size, SPHERE_DIV, shape.color, shape.color, FALSE);
			break;
		case SHAPE::ARROW:
			DrawArrow(shape);
			break;
		case SHAPE::POINT:
			DrawPoint(shape);
			break;
		}
	}
#endif
}

void DebugDraw3D::Clear(void)
{
#if USE_DEBUG_TOOLS
	shapes.clear();
#endif
}