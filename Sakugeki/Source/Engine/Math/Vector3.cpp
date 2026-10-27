#include <cmath>
#include <algorithm>
#include "Vector3.h"

Vector3::Vector3(void)
	:
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Vector3::Vector3(float vX, float vY, float vZ)
	:
	x(vX),
	y(vY),
	z(vZ)
{
}

Vector3::Vector3(const VECTOR& v)
	:
	x(v.x),
	y(v.y),
	z(v.z)
{
}

Vector3::operator VECTOR(void) const
{
	return VECTOR{ x, y, z };
}

Vector3 Vector3::operator-(void) const
{
	return { -x, -y, -z };
}

Vector3 Vector3::operator+(const Vector3& v) const
{
	return { x + v.x, y + v.y, z + v.z };
}

Vector3 Vector3::operator-(const Vector3& v) const
{
	return { x - v.x, y - v.y, z - v.z };
}

Vector3 Vector3::operator*(float s) const
{
	return { x * s, y * s, z * s };
}

Vector3 Vector3::operator/(float s) const
{
	return { x / s, y / s, z / s };
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

float Vector3::Magnitude(void) const
{
	return std::sqrt(SquareMagnitude());
}

float Vector3::SquareMagnitude(void) const
{
	return x * x + y * y + z * z;
}

Vector3 Vector3::Normalized(void) const
{
	float sqrLen = SquareMagnitude();

	// 長さ0のベクトルは正規化できないので ZERO を返す
	if (sqrLen <= EPSILON * EPSILON)
	{
		return ZERO;
	}

	return *this / std::sqrt(sqrLen);
}

bool Vector3::IsZero(void) const
{
	return SquareMagnitude() <= EPSILON * EPSILON;
}

float Vector3::Dot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3::Cross(const Vector3& a, const Vector3& b)
{
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x };
}

Vector3 Vector3::Scale(const Vector3& a, const Vector3& b)
{
	return { a.x * b.x, a.y * b.y, a.z * b.z };
}

Vector3 Vector3::Lerp(const Vector3& start, const Vector3& end, float t)
{
	return start + (end - start) * t;
}

Vector3 Vector3::Slerp(const Vector3& start, const Vector3& end, float t)
{
	float lenStart = start.Magnitude();
	float lenEnd = end.Magnitude();

	// どちらかが長さ0なら方向が決まらないので線形補間
	if (lenStart <= EPSILON || lenEnd <= EPSILON)
	{
		return Lerp(start, end, t);
	}

	// 方向だけを取り出す
	Vector3 from = start / lenStart;
	Vector3 to = end / lenEnd;

	// 2方向のなす角（誤差で acos の範囲外にならないようにクランプ）
	float dot = (std::max)(-1.0f, (std::min)(1.0f, Dot(from, to)));
	float angle = std::acos(dot);

	// ほぼ同じ方向なら線形補間
	constexpr float MIN_ANGLE = 1.0e-4f;
	if (angle < MIN_ANGLE)
	{
		return Lerp(start, end, t);
	}

	// from に垂直で、to 側を向く単位ベクトル
	Vector3 ortho = (to - from * dot).Normalized();

	// 真逆の方向なら回転面が決まらないので、from に垂直な適当な軸を選ぶ
	if (ortho.IsZero())
	{
		ortho = Cross(from, UP).Normalized();
		if (ortho.IsZero())
		{
			ortho = Cross(from, RIGHT).Normalized();
		}
	}

	// 方向は角度で、長さは線形で補間する
	float rad = angle * t;
	Vector3 dir = from * std::cos(rad) + ortho * std::sin(rad);
	float len = lenStart + (lenEnd - lenStart) * t;

	return dir * len;
}

float Vector3::Distance(const Vector3& a, const Vector3& b)
{
	return (a - b).Magnitude();
}

bool Vector3::Equals(const Vector3& a, const Vector3& b, float eps)
{
	return (a - b).SquareMagnitude() <= eps * eps;
}

Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}