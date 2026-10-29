#pragma once
#include <DxLib.h>
#include "Vector3.h"

// 3D 用の 4x4 行列
struct Matrix3D
{
	float m[4][4];

	// 単位行列
	static const Matrix3D IDENTITY;

	// コンストラクタ（単位行列）
	Matrix3D(void);

	// 行列の合成
	Matrix3D operator*(const Matrix3D& mat) const;
	Matrix3D& operator*=(const Matrix3D& mat);

	// 座標を変換する（平行移動を含む）
	Vector3 TransformPoint(const Vector3& pos) const;

	// 方向を変換する（平行移動を含まない）
	Vector3 TransformDirection(const Vector3& dir) const;

	// 逆行列
	Matrix3D Inverse(void) const;

	// 転置行列
	Matrix3D Transpose(void) const;

	// 平行移動成分
	Vector3 GetTranslation(void) const;

	// 拡大縮小行列を作る
	static Matrix3D Scale(const Vector3& scale);

	// 平行移動行列を作る
	static Matrix3D Translate(const Vector3& pos);

	// DxLib の MATRIX との変換
	MATRIX ToMATRIX(void) const;
	static Matrix3D GetMatrix3D(const MATRIX& mat);
};

// DxLib の MATRIX とサイズが同じであることを保証
static_assert(sizeof(Matrix3D) == sizeof(MATRIX), "Matrix3D と MATRIX のサイズが一致しません");

// 定数の定義
inline const Matrix3D Matrix3D::IDENTITY = Matrix3D();
