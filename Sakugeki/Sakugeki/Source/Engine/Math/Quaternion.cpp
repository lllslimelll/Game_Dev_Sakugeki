#include <cmath>
#include <algorithm>
#include "UtilityMath.h"
#include "Quaternion.h"

Quaternion::Quaternion(void)
	:
	w(1.0f),
	x(0.0f),
	y(0.0f),
	z(0.0f)
{
}

Quaternion::Quaternion(float qW, float qX, float qY, float qZ)
	:
	w(qW),
	x(qX),
	y(qY),
	z(qZ)
{
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	// ハミルトン積
	return {
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + q.w * x + y * q.z - z * q.y,
		w * q.y + q.w * y + z * q.x - x * q.z,
		w * q.z + q.w * z + x * q.y - y * q.x };
}

Vector3 Quaternion::Rotate(const Vector3& vec) const
{
	Quaternion ret = *this * Quaternion(0.0f, vec.x, vec.y, vec.z) * Inverse();
	return { ret.x, ret.y, ret.z };
}

Vector3 Quaternion::ToEuler(void) const
{
	float r11 = 2.0f * (x * z + w * y);
	float r12 = w * w - x * x - y * y + z * z;
	float r21 = -2.0f * (y * z - w * x);
	float r31 = 2.0f * (x * y + w * z);
	float r32 = w * w - x * x + y * y - z * z;

	// 誤差で asin の範囲外にならないようにクランプ
	r21 = (std::max)(-1.0f, (std::min)(1.0f, r21));

	return {
		std::asin(r21),
		std::atan2(r11, r12),
		std::atan2(r31, r32) };
}

Quaternion Quaternion::Normalized(void) const
{
	float mag = Magnitude();

	// 長さ0は正規化できないので回転なしを返す
	if (mag <= EPSILON)
	{
		return IDENTITY;
	}

	return { w / mag, x / mag, y / mag, z / mag };
}

Quaternion Quaternion::Inverse(void) const
{
	// 共役を長さの2乗で割る
	float sqrMag = SquareMagnitude();
	if (sqrMag <= EPSILON * EPSILON)
	{
		return IDENTITY;
	}

	float inv = 1.0f / sqrMag;
	return { w * inv, -x * inv, -y * inv, -z * inv };
}

float Quaternion::Magnitude(void) const
{
	return std::sqrt(SquareMagnitude());
}

float Quaternion::SquareMagnitude(void) const
{
	return w * w + x * x + y * y + z * z;
}

Vector3 Quaternion::GetForward(void) const
{
	return Rotate(Vector3::DIR_FORWARD);
}

Vector3 Quaternion::GetBack(void) const
{
	return Rotate(Vector3::DIR_BACK);
}

Vector3 Quaternion::GetRight(void) const
{
	return Rotate(Vector3::DIR_RIGHT);
}

Vector3 Quaternion::GetLeft(void) const
{
	return Rotate(Vector3::DIR_LEFT);
}

Vector3 Quaternion::GetUp(void) const
{
	return Rotate(Vector3::DIR_UP);
}

Vector3 Quaternion::GetDown(void) const
{
	return Rotate(Vector3::DIR_DOWN);
}

void Quaternion::ToAngleAxis(float& rad, Vector3& axis) const
{
	// 自分自身は書き換えず、正規化したコピーで計算する
	Quaternion q = *this;
	if (std::fabs(q.w) > 1.0f)
	{
		q = q.Normalized();
	}

	rad = 2.0f * std::acos(q.w);

	float den = std::sqrt(1.0f - q.w * q.w);
	if (den > 0.0001f)
	{
		axis = Vector3(q.x, q.y, q.z) / den;
	}
	else
	{
		// 回転角がほぼ0のときは軸が決まらないので、適当な単位ベクトルにする
		rad = 0.0f;
		axis = Vector3::DIR_RIGHT;
	}
}

Quaternion Quaternion::Euler(const Vector3& rad)
{
	return Euler(rad.x, rad.y, rad.z);
}

Quaternion Quaternion::Euler(float radX, float radY, float radZ)
{
	radX = UtilityMath::RadIn2PIF(radX);
	radY = UtilityMath::RadIn2PIF(radY);
	radZ = UtilityMath::RadIn2PIF(radZ);

	float cosX = std::cos(radX * 0.5f);
	float sinX = std::sin(radX * 0.5f);
	float cosY = std::cos(radY * 0.5f);
	float sinY = std::sin(radY * 0.5f);
	float cosZ = std::cos(radZ * 0.5f);
	float sinZ = std::sin(radZ * 0.5f);

	return {
		cosX * cosY * cosZ + sinX * sinY * sinZ,
		sinX * cosY * cosZ + cosX * sinY * sinZ,
		cosX * sinY * cosZ - sinX * cosY * sinZ,
		cosX * cosY * sinZ - sinX * sinY * cosZ };
}

Quaternion Quaternion::AngleAxis(float rad, const Vector3& axis)
{
	Vector3 n = axis.Normalized();

	// 軸が決まらないときは回転なし
	if (n.IsZero())
	{
		return IDENTITY;
	}

	float c = std::cos(rad * 0.5f);
	float s = std::sin(rad * 0.5f);

	return { c, s * n.x, s * n.y, s * n.z };
}

Quaternion Quaternion::LookRotation(const Vector3& forward)
{
	return LookRotation(forward, Vector3::DIR_UP);
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up)
{
	// 1. 正規化
	Vector3 zAxis = forward.Normalized();
	Vector3 u = up.Normalized();

	// 向く方向が決まらないときは回転なし
	if (zAxis.IsZero())
	{
		return IDENTITY;
	}

	// 2. up と forward がほぼ平行なときは、別の軸を上にする
	if (std::fabs(Vector3::Dot(u, zAxis)) > 0.999f)
	{
		u = Vector3::DIR_UP;
		if (std::fabs(Vector3::Dot(u, zAxis)) > 0.999f)
		{
			u = Vector3::DIR_RIGHT;
		}
	}

	// 3. 直交する3軸を作る
	Vector3 xAxis = Vector3::Cross(u, zAxis).Normalized();
	Vector3 yAxis = Vector3::Cross(zAxis, xAxis).Normalized();

	// 4. 回転行列の要素
	float m00 = xAxis.x, m01 = yAxis.x, m02 = zAxis.x;
	float m10 = xAxis.y, m11 = yAxis.y, m12 = zAxis.y;
	float m20 = xAxis.z, m21 = yAxis.z, m22 = zAxis.z;

	// 5. 回転行列からクォータニオンへ
	float trace = m00 + m11 + m22;
	Quaternion q;

	if (trace > 0.0f)
	{
		float num = std::sqrt(trace + 1.0f);
		q.w = num * 0.5f;
		num = 0.5f / num;
		q.x = (m21 - m12) * num;
		q.y = (m02 - m20) * num;
		q.z = (m10 - m01) * num;
	}
	else if ((m00 >= m11) && (m00 >= m22))
	{
		float num = std::sqrt(1.0f + m00 - m11 - m22);
		float inv = 0.5f / num;
		q.x = 0.5f * num;
		q.y = (m10 + m01) * inv;
		q.z = (m02 + m20) * inv;
		q.w = (m21 - m12) * inv;
	}
	else if (m11 >= m22)
	{
		float num = std::sqrt(1.0f + m11 - m00 - m22);
		float inv = 0.5f / num;
		q.x = (m10 + m01) * inv;
		q.y = 0.5f * num;
		q.z = (m21 + m12) * inv;
		q.w = (m02 - m20) * inv;
	}
	else
	{
		float num = std::sqrt(1.0f + m22 - m00 - m11);
		float inv = 0.5f / num;
		q.x = (m02 + m20) * inv;
		q.y = (m21 + m12) * inv;
		q.z = 0.5f * num;
		q.w = (m10 - m01) * inv;
	}

	return q;
}

Quaternion Quaternion::FromToRotation(const Vector3& fromDir, const Vector3& toDir)
{
	Vector3 from = fromDir.Normalized();
	Vector3 to = toDir.Normalized();

	// どちらかの方向が決まらないときは回転なし
	if (from.IsZero() || to.IsZero())
	{
		return IDENTITY;
	}

	// 2方向のなす角(rad)
	float dot = (std::max)(-1.0f, (std::min)(1.0f, Vector3::Dot(from, to)));
	float rad = std::acos(dot);

	// 回転軸
	Vector3 axis = Vector3::Cross(from, to);

	// ほぼ真逆のときは軸が決まらないので、from に垂直な軸を選ぶ
	constexpr float OPPOSITE_RAD = 179.9196f * UtilityMath::DEG2RAD_F;
	if (rad >= OPPOSITE_RAD)
	{
		axis = Vector3::Cross(Vector3::Cross(from, Vector3::DIR_RIGHT), from);
		if (axis.IsZero())
		{
			axis = Vector3::DIR_UP;
		}
	}

	return AngleAxis(rad, axis);
}

Quaternion Quaternion::RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta)
{
	float angle = Angle(from, to);
	if (angle <= 0.0f)
	{
		return to;
	}

	float t = (std::min)(1.0f, maxDegreesDelta / angle);
	return SlerpUnclamped(from, to, t);
}

float Quaternion::Angle(const Quaternion& a, const Quaternion& b)
{
	// q と -q は同じ回転なので絶対値を取る
	float dot = std::fabs(Dot(a, b));

	// ほぼ同じ回転なら0（float の誤差で 0.04 度などが出るのを防ぐ）
	if (dot >= 1.0f - EPSILON)
	{
		return 0.0f;
	}

	// クォータニオンの内積は半角の cos なので2倍する
	return 2.0f * std::acos(dot) * UtilityMath::RAD2DEG_F;
}

float Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
	return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

Quaternion Quaternion::Slerp(const Quaternion& from, const Quaternion& to, float t)
{
	t = (std::max)(0.0f, (std::min)(1.0f, t));
	return SlerpUnclamped(from, to, t);
}

Quaternion Quaternion::SlerpUnclamped(const Quaternion& from, const Quaternion& to, float t)
{
	Quaternion a = from;
	Quaternion b = to;

	// どちらかが長さ0なら、もう一方を返す
	if (a.SquareMagnitude() == 0.0f)
	{
		if (b.SquareMagnitude() == 0.0f)
		{
			return IDENTITY;
		}
		return b;
	}
	else if (b.SquareMagnitude() == 0.0f)
	{
		return a;
	}

	float cosHalfAngle = Dot(a, b);

	if (cosHalfAngle >= 1.0f || cosHalfAngle <= -1.0f)
	{
		// 角度0なので、そのまま返す
		return a;
	}
	else if (cosHalfAngle < 0.0f)
	{
		// 遠回りしないように、b を反転して近い側を通る
		b = { -b.w, -b.x, -b.y, -b.z };
		cosHalfAngle = -cosHalfAngle;
	}

	float blendA;
	float blendB;
	if (cosHalfAngle < 0.99f)
	{
		// 角度が大きいときは球面補間
		float halfAngle = std::acos(cosHalfAngle);
		float invSinHalfAngle = 1.0f / std::sin(halfAngle);
		blendA = std::sin(halfAngle * (1.0f - t)) * invSinHalfAngle;
		blendB = std::sin(halfAngle * t) * invSinHalfAngle;
	}
	else
	{
		// 角度がとても小さいときは線形補間
		blendA = 1.0f - t;
		blendB = t;
	}

	Quaternion result = {
		blendA * a.w + blendB * b.w,
		blendA * a.x + blendB * b.x,
		blendA * a.y + blendB * b.y,
		blendA * a.z + blendB * b.z };

	return result.Normalized();
}

Matrix3D Quaternion::ToMatrix3D(void) const
{
	// DxLib と同じ行ベクトル規約の回転行列
	Matrix3D mat;

	float sx = x * x * 2.0f;
	float sy = y * y * 2.0f;
	float sz = z * z * 2.0f;
	float cx = y * z * 2.0f;
	float cy = x * z * 2.0f;
	float cz = x * y * 2.0f;
	float wx = w * x * 2.0f;
	float wy = w * y * 2.0f;
	float wz = w * z * 2.0f;

	mat.m[0][0] = 1.0f - (sy + sz);	mat.m[0][1] = cz + wz;			mat.m[0][2] = cy - wy;			mat.m[0][3] = 0.0f;
	mat.m[1][0] = cz - wz;			mat.m[1][1] = 1.0f - (sx + sz);	mat.m[1][2] = cx + wx;			mat.m[1][3] = 0.0f;
	mat.m[2][0] = cy + wy;			mat.m[2][1] = cx - wx;			mat.m[2][2] = 1.0f - (sx + sy);	mat.m[2][3] = 0.0f;
	mat.m[3][0] = 0.0f;				mat.m[3][1] = 0.0f;				mat.m[3][2] = 0.0f;				mat.m[3][3] = 1.0f;

	return mat;
}

Quaternion Quaternion::GetQuaternion(const Matrix3D& mat)
{
	Quaternion ret;

	float s;
	float tr = mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f;
	if (tr >= 1.0f)
	{
		s = 0.5f / std::sqrt(tr);
		ret.w = 0.25f / s;
		ret.x = (mat.m[1][2] - mat.m[2][1]) * s;
		ret.y = (mat.m[2][0] - mat.m[0][2]) * s;
		ret.z = (mat.m[0][1] - mat.m[1][0]) * s;
	}
	else
	{
		float max = mat.m[1][1] > mat.m[2][2] ? mat.m[1][1] : mat.m[2][2];

		if (max < mat.m[0][0])
		{
			s = std::sqrt(mat.m[0][0] - (mat.m[1][1] + mat.m[2][2]) + 1.0f);
			float qx = s * 0.5f;
			s = 0.5f / s;
			ret.x = qx;
			ret.y = (mat.m[0][1] + mat.m[1][0]) * s;
			ret.z = (mat.m[2][0] + mat.m[0][2]) * s;
			ret.w = (mat.m[1][2] - mat.m[2][1]) * s;
		}
		else if (max == mat.m[1][1])
		{
			s = std::sqrt(mat.m[1][1] - (mat.m[2][2] + mat.m[0][0]) + 1.0f);
			float qy = s * 0.5f;
			s = 0.5f / s;
			ret.x = (mat.m[0][1] + mat.m[1][0]) * s;
			ret.y = qy;
			ret.z = (mat.m[1][2] + mat.m[2][1]) * s;
			ret.w = (mat.m[2][0] - mat.m[0][2]) * s;
		}
		else
		{
			s = std::sqrt(mat.m[2][2] - (mat.m[0][0] + mat.m[1][1]) + 1.0f);
			float qz = s * 0.5f;
			s = 0.5f / s;
			ret.x = (mat.m[2][0] + mat.m[0][2]) * s;
			ret.y = (mat.m[1][2] + mat.m[2][1]) * s;
			ret.z = qz;
			ret.w = (mat.m[0][1] - mat.m[1][0]) * s;
		}
	}

	return ret;
}
