#include <cmath>
#include "Vector2.h"

Vector2::Vector2(void)
	:
	x(0.0f),
	y(0.0f)
{
}

Vector2::Vector2(float vecX, float vecY)
	:
	x(vecX),
	y(vecY)
{
}

Vector2 Vector2::operator+(const Vector2& vec) const
{
	return { x + vec.x, y + vec.y };
}

Vector2 Vector2::operator-(const Vector2& vec) const
{
	return { x - vec.x, y - vec.y };
}

Vector2 Vector2::operator*(float scale) const
{
	return { x * scale, y * scale };
}

Vector2 Vector2::operator/(float div) const
{
	return { x / div, y / div };
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2& Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	return *this;
}

Vector2& Vector2::operator/=(float div)
{
	x /= div;
	y /= div;
	return *this;
}

float Vector2::Magnitude(void) const
{
	return std::sqrt(SquareMagnitude());
}

float Vector2::SquareMagnitude(void) const
{
	return x * x + y * y;
}

Vector2 Vector2::Normalized(void) const
{
	float sqrLen = SquareMagnitude();

	// 長さ0のベクトルは正規化できないので ZERO を返す
	if (sqrLen <= EPSILON * EPSILON)
	{
		return ZERO;
	}

	return *this / std::sqrt(sqrLen);
}

bool Vector2::IsZero(void) const
{
	return SquareMagnitude() <= EPSILON * EPSILON;
}

float Vector2::Dot(const Vector2& a, const Vector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2::Cross(const Vector2& a, const Vector2& b)
{
	return a.x * b.y - a.y * b.x;
}

Vector2 Vector2::Lerp(const Vector2& start, const Vector2& end, float t)
{
	return start + (end - start) * t;
}

float Vector2::Distance(const Vector2& a, const Vector2& b)
{
	return (a - b).Magnitude();
}

bool Vector2::Equals(const Vector2& a, const Vector2& b, float eps)
{
	return (a - b).SquareMagnitude() <= eps * eps;
}
