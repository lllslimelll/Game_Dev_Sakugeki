#include <cmath>
#include "Matrix3D.h"

Matrix3D::Matrix3D(void)
	:
	m{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f } }
{
}

Matrix3D Matrix3D::operator*(const Matrix3D& mat) const
{
	Matrix3D ret;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			ret.m[row][col] =
				m[row][0] * mat.m[0][col] +
				m[row][1] * mat.m[1][col] +
				m[row][2] * mat.m[2][col] +
				m[row][3] * mat.m[3][col];
		}
	}
	return ret;
}

Matrix3D& Matrix3D::operator*=(const Matrix3D& mat)
{
	*this = *this * mat;
	return *this;
}

Vector3 Matrix3D::TransformPoint(const Vector3& pos) const
{
	return {
		pos.x * m[0][0] + pos.y * m[1][0] + pos.z * m[2][0] + m[3][0],
		pos.x * m[0][1] + pos.y * m[1][1] + pos.z * m[2][1] + m[3][1],
		pos.x * m[0][2] + pos.y * m[1][2] + pos.z * m[2][2] + m[3][2] };
}

Vector3 Matrix3D::TransformDirection(const Vector3& dir) const
{
	return {
		dir.x * m[0][0] + dir.y * m[1][0] + dir.z * m[2][0],
		dir.x * m[0][1] + dir.y * m[1][1] + dir.z * m[2][1],
		dir.x * m[0][2] + dir.y * m[1][2] + dir.z * m[2][2] };
}

Matrix3D Matrix3D::Inverse(void) const
{
	// 余因子展開による 4x4 の逆行列
	const float* a = &m[0][0];
	float inv[16];

	inv[0] = a[5] * a[10] * a[15] - a[5] * a[11] * a[14] - a[9] * a[6] * a[15]
		+ a[9] * a[7] * a[14] + a[13] * a[6] * a[11] - a[13] * a[7] * a[10];
	inv[4] = -a[4] * a[10] * a[15] + a[4] * a[11] * a[14] + a[8] * a[6] * a[15]
		- a[8] * a[7] * a[14] - a[12] * a[6] * a[11] + a[12] * a[7] * a[10];
	inv[8] = a[4] * a[9] * a[15] - a[4] * a[11] * a[13] - a[8] * a[5] * a[15]
		+ a[8] * a[7] * a[13] + a[12] * a[5] * a[11] - a[12] * a[7] * a[9];
	inv[12] = -a[4] * a[9] * a[14] + a[4] * a[10] * a[13] + a[8] * a[5] * a[14]
		- a[8] * a[6] * a[13] - a[12] * a[5] * a[10] + a[12] * a[6] * a[9];
	inv[1] = -a[1] * a[10] * a[15] + a[1] * a[11] * a[14] + a[9] * a[2] * a[15]
		- a[9] * a[3] * a[14] - a[13] * a[2] * a[11] + a[13] * a[3] * a[10];
	inv[5] = a[0] * a[10] * a[15] - a[0] * a[11] * a[14] - a[8] * a[2] * a[15]
		+ a[8] * a[3] * a[14] + a[12] * a[2] * a[11] - a[12] * a[3] * a[10];
	inv[9] = -a[0] * a[9] * a[15] + a[0] * a[11] * a[13] + a[8] * a[1] * a[15]
		- a[8] * a[3] * a[13] - a[12] * a[1] * a[11] + a[12] * a[3] * a[9];
	inv[13] = a[0] * a[9] * a[14] - a[0] * a[10] * a[13] - a[8] * a[1] * a[14]
		+ a[8] * a[2] * a[13] + a[12] * a[1] * a[10] - a[12] * a[2] * a[9];
	inv[2] = a[1] * a[6] * a[15] - a[1] * a[7] * a[14] - a[5] * a[2] * a[15]
		+ a[5] * a[3] * a[14] + a[13] * a[2] * a[7] - a[13] * a[3] * a[6];
	inv[6] = -a[0] * a[6] * a[15] + a[0] * a[7] * a[14] + a[4] * a[2] * a[15]
		- a[4] * a[3] * a[14] - a[12] * a[2] * a[7] + a[12] * a[3] * a[6];
	inv[10] = a[0] * a[5] * a[15] - a[0] * a[7] * a[13] - a[4] * a[1] * a[15]
		+ a[4] * a[3] * a[13] + a[12] * a[1] * a[7] - a[12] * a[3] * a[5];
	inv[14] = -a[0] * a[5] * a[14] + a[0] * a[6] * a[13] + a[4] * a[1] * a[14]
		- a[4] * a[2] * a[13] - a[12] * a[1] * a[6] + a[12] * a[2] * a[5];
	inv[3] = -a[1] * a[6] * a[11] + a[1] * a[7] * a[10] + a[5] * a[2] * a[11]
		- a[5] * a[3] * a[10] - a[9] * a[2] * a[7] + a[9] * a[3] * a[6];
	inv[7] = a[0] * a[6] * a[11] - a[0] * a[7] * a[10] - a[4] * a[2] * a[11]
		+ a[4] * a[3] * a[10] + a[8] * a[2] * a[7] - a[8] * a[3] * a[6];
	inv[11] = -a[0] * a[5] * a[11] + a[0] * a[7] * a[9] + a[4] * a[1] * a[11]
		- a[4] * a[3] * a[9] - a[8] * a[1] * a[7] + a[8] * a[3] * a[5];
	inv[15] = a[0] * a[5] * a[10] - a[0] * a[6] * a[9] - a[4] * a[1] * a[10]
		+ a[4] * a[2] * a[9] + a[8] * a[1] * a[6] - a[8] * a[2] * a[5];

	float det = a[0] * inv[0] + a[1] * inv[4] + a[2] * inv[8] + a[3] * inv[12];

	// 行列式が0（拡大率0など）のときは逆行列が存在しない
	constexpr float MIN_DET = 1.0e-12f;
	if (std::fabs(det) <= MIN_DET)
	{
		return IDENTITY;
	}

	float invDet = 1.0f / det;
	Matrix3D ret;
	for (int i = 0; i < 16; i++)
	{
		ret.m[i / 4][i % 4] = inv[i] * invDet;
	}
	return ret;
}

Matrix3D Matrix3D::Transpose(void) const
{
	Matrix3D ret;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			ret.m[row][col] = m[col][row];
		}
	}
	return ret;
}

Vector3 Matrix3D::GetTranslation(void) const
{
	return { m[3][0], m[3][1], m[3][2] };
}

Matrix3D Matrix3D::Scale(const Vector3& scale)
{
	Matrix3D ret;
	ret.m[0][0] = scale.x;
	ret.m[1][1] = scale.y;
	ret.m[2][2] = scale.z;
	return ret;
}

Matrix3D Matrix3D::Translate(const Vector3& pos)
{
	Matrix3D ret;
	ret.m[3][0] = pos.x;
	ret.m[3][1] = pos.y;
	ret.m[3][2] = pos.z;
	return ret;
}

MATRIX Matrix3D::ToMATRIX(void) const
{
	MATRIX ret;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			ret.m[row][col] = m[row][col];
		}
	}
	return ret;
}

Matrix3D Matrix3D::GetMatrix3D(const MATRIX& mat)
{
	Matrix3D ret;
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			ret.m[row][col] = mat.m[row][col];
		}
	}
	return ret;
}
