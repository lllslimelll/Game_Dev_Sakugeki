#pragma once

// 2次元ベクトル（画面座標・UI・2D の計算用）
struct Vector2
{
	float x;
	float y;

	// 長さ0とみなす閾値
	static constexpr float EPSILON = 1.0e-6f;

	// 頻出する成分の定数
	static const Vector2 ZERO;
	static const Vector2 ONE;

	// コンストラクタ
	Vector2(void);
	Vector2(float vecX, float vecY);

	// 二項演算子
	Vector2 operator+(const Vector2& vec) const;
	Vector2 operator-(const Vector2& vec) const;
	Vector2 operator*(float scale) const;
	Vector2 operator/(float div) const;

	// 複合代入演算子
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(float scale);
	Vector2& operator/=(float div);

	// 長さ
	float Magnitude(void) const;

	// 長さの2乗
	float SquareMagnitude(void) const;

	// 正規化したベクトル
	Vector2 Normalized(void) const;

	// 長さが0か
	bool IsZero(void) const;

	// 内積
	static float Dot(const Vector2& a, const Vector2& b);

	// 外積
	static float Cross(const Vector2& a, const Vector2& b);

	// 線形補間
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	// 2点間の距離
	static float Distance(const Vector2& a, const Vector2& b);

	// 比較
	static bool Equals(const Vector2& a, const Vector2& b, float eps = EPSILON);
};

// 頻出する成分の定数の定義
inline const Vector2 Vector2::ZERO = { 0.0f, 0.0f };
inline const Vector2 Vector2::ONE = { 1.0f, 1.0f };
