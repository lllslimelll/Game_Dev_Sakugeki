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

Vector3::Vector3(float vecX, float vecY, float vecZ)
	:
	x(vecX),
	y(vecY),
	z(vecZ)
{
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	return { x + vec.x, y + vec.y, z + vec.z };
}

Vector3 Vector3::operator-(const Vector3& vec) const
{
	return { x - vec.x, y - vec.y, z - vec.z };
}

Vector3 Vector3::operator*(float scale) const
{
	return { x * scale, y * scale, z * scale };
}

Vector3 Vector3::operator/(float div) const
{
	return { x / div, y / div, z / div };
}

Vector3& Vector3::operator+=(const Vector3& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector3& Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

Vector3& Vector3::operator/=(float div)
{
	x /= div;
	y /= div;
	z /= div;
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
		ortho = Cross(from, DIR_UP).Normalized();
		if (ortho.IsZero())
		{
			ortho = Cross(from, DIR_RIGHT).Normalized();
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

VECTOR Vector3::ToVECTOR() const
{
	return VGet(x, y, z);
}

Vector3 Vector3::GetVector3(const VECTOR& vec)
{
	return { vec.x, vec.y, vec.z };
}