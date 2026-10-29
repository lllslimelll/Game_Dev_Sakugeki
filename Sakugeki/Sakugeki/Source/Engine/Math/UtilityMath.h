#pragma once

// 数学系の共通関数・定数
namespace UtilityMath
{
	// 円周率
	constexpr float PI_F = 3.14159265358979323846f;
	constexpr double PI_D = 3.14159265358979323846;
	// 円周率の2倍
	constexpr float TWO_PI_F = PI_F * 2.0f;

	// ラジアン(rad)・度(deg)変換用の係数
	constexpr float RAD2DEG_F = 180.0f / PI_F;
	constexpr float DEG2RAD_F = PI_F / 180.0f;

	// 度(deg)からラジアン(rad)
	float DegToRadF(float deg);
	double DegToRadD(double deg);

	// ラジアン(rad)から度(deg)
	float RadToDegF(float rad);
	double RadToDegD(double rad);

	// 0〜2π の範囲に収める
	float RadIn2PIF(float rad);
}