#pragma once
#include <DxLib.h>

// 3次元ベクトル
struct Vector3
{
	float x;
	float y;
	float z;

	// 長さ0とみなす閾値
	static constexpr float EPSILON = 1.0e-6f;

	// 頻出する成分の定数
	static const Vector3 ZERO;
	static const Vector3 ONE;
	static const Vector3 DIR_UP;
	static const Vector3 DIR_DOWN;
	static const Vector3 DIR_RIGHT;
	static const Vector3 DIR_LEFT;
	static const Vector3 DIR_FORWARD;
	static const Vector3 DIR_BACK;

	// コンストラクタ
	Vector3(void);

	// コンストラクタ
	Vector3(float vecX, float vecY, float vecZ);

	// 二項演算子
	Vector3 operator+(const Vector3& vec) const;
	Vector3 operator-(const Vector3& vec) const;
	Vector3 operator*(float scale) const;
	Vector3 operator/(float div) const;

	// 複合代入演算子
	Vector3& operator+=(const Vector3& vec);
	Vector3& operator-=(const Vector3& vec);
	Vector3& operator*=(float scale);
	Vector3& operator/=(float div);

	// 長さ
	float Magnitude(void) const;

	// 長さの2乗（比較だけなら sqrt 不要でこちらが速い）
	float SquareMagnitude(void) const;

	// 正規化したベクトル
	Vector3 Normalized(void) const;

	// 長さが0か
	bool IsZero(void) const;

	// 内積
	static float Dot(const Vector3& a, const Vector3& b);

	// 外積
	static Vector3 Cross(const Vector3& a, const Vector3& b);

	// 線形補間
	static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
	// 球面線形補間
	static Vector3 Slerp(const Vector3& start, const Vector3& end, float t);

	// 2点間の距離
	static float Distance(const Vector3& a, const Vector3& b);

	// 比較
	static bool Equals(const Vector3& a, const Vector3& b, float eps = EPSILON);

	// DxLibの VECTOR への変換
	VECTOR ToVECTOR() const;
	// DxLibの VECTOR からVector3に変換した成分を取得
	static Vector3 GetVector3(const VECTOR& vec);
};

// DxLibのVECTORとサイズが同じであることを保証
static_assert(sizeof(Vector3) == sizeof(VECTOR), "Vector3 と VECTOR のサイズが一致しません");

// 頻出する成分の定数の定義
inline const Vector3 Vector3::ZERO = { 0.0f,  0.0f,  0.0f };
inline const Vector3 Vector3::ONE = { 1.0f,  1.0f,  1.0f };
inline const Vector3 Vector3::DIR_UP = { 0.0f,  1.0f,  0.0f };
inline const Vector3 Vector3::DIR_DOWN = { 0.0f, -1.0f,  0.0f };
inline const Vector3 Vector3::DIR_RIGHT = { 1.0f,  0.0f,  0.0f };
inline const Vector3 Vector3::DIR_LEFT = { -1.0f,  0.0f,  0.0f };
inline const Vector3 Vector3::DIR_FORWARD = { 0.0f,  0.0f,  1.0f };
inline const Vector3 Vector3::DIR_BACK = { 0.0f,  0.0f, -1.0f };

