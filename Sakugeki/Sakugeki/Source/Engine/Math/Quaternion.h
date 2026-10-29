#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "Matrix3D.h"

// クォータニオン
struct Quaternion
{
	float w;
	float x;
	float y;
	float z;

	// 長さ0とみなす閾値
	static constexpr float EPSILON = 1.0e-6f;

	// 回転なし
	static const Quaternion IDENTITY;

	// コンストラクタ
	Quaternion(void);
	Quaternion(float qW, float qX, float qY, float qZ);

	// 回転の合成
	Quaternion operator*(const Quaternion& q) const;

	// ベクトルを回転させる
	Vector3 Rotate(const Vector3& vec) const;

	// オイラー角（rad）に変換
	Vector3 ToEuler(void) const;

	// 正規化
	Quaternion Normalized(void) const;

	// 逆クォータニオン
	Quaternion Inverse(void) const;

	// 長さ
	float Magnitude(void) const;

	// 長さの2乗
	float SquareMagnitude(void) const;

	// 基本方向を取得
	Vector3 GetForward(void) const;
	Vector3 GetBack(void) const;
	Vector3 GetRight(void) const;
	Vector3 GetLeft(void) const;
	Vector3 GetUp(void) const;
	Vector3 GetDown(void) const;

	// 回転角（rad）と回転軸に分解する
	void ToAngleAxis(float& rad, Vector3& axis) const;

	// オイラー角（rad）から作る
	static Quaternion Euler(const Vector3& rad);
	static Quaternion Euler(float radX, float radY, float radZ);

	// 指定軸まわりに指定角（rad）回転するクォータニオンを作る
	static Quaternion AngleAxis(float rad, const Vector3& axis);

	// 指定方向を向くクォータニオンを作成
	static Quaternion LookRotation(const Vector3& forward);
	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

	// fromDir を toDir に向ける回転を作る
	static Quaternion FromToRotation(const Vector3& fromDir, const Vector3& toDir);

	// from から to へ、最大 maxDegree だけ回転させる
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegree);

	// 2つの回転の差の角度（deg）
	static float Angle(const Quaternion& a, const Quaternion& b);

	// 内積
	static float Dot(const Quaternion& a, const Quaternion& b);

	// 球面線形補間（t は 0〜1 にクランプ）
	static Quaternion Slerp(const Quaternion& from, const Quaternion& to, float t);

	// 球面線形補間（t をクランプしない）
	static Quaternion SlerpUnclamped(const Quaternion& from, const Quaternion& to, float t);

	// 回転行列との変換
	Matrix3D ToMatrix3D(void) const;
	static Quaternion GetQuaternion(const Matrix3D& mat);
};

// 回転無し
inline const Quaternion Quaternion::IDENTITY = { 1.0f, 0.0f, 0.0f, 0.0f };
