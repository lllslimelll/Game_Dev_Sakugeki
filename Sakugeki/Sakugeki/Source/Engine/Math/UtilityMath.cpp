#include <cmath>
#include "UtilityMath.h"

namespace UtilityMath
{
	float DegToRadF(float deg)
	{
		return deg * (PI_F / 180.0f);
	}

	double DegToRadD(double deg)
	{
		return deg * (PI_D / 180.0);
	}

	float RadToDegF(float rad)
	{
		return rad * (180.0f / PI_F);
	}

	double RadToDegD(double rad)
	{
		return rad * (180.0 / PI_D);
	}
	float RadIn2PIF(float rad)
	{
		rad = std::fmod(rad, TWO_PI_F);
		if (rad < 0.0f)
		{
			rad += TWO_PI_F;
		}
		return rad;

	}
}