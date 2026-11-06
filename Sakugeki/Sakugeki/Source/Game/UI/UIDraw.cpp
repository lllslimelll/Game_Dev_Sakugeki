#include <algorithm>
#include <DxLib.h>
#include "UIDraw.h"

void UIDraw::SetAlpha(int alpha)
{
	if (alpha >= 255)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (std::max)(0, alpha));
	}
}

unsigned int UIDraw::LerpColor(unsigned int a, unsigned int b, float t)
{
	auto channel = [t](unsigned int ca, unsigned int cb)
		{
			const float v = static_cast<float>(ca) + (static_cast<float>(cb) - static_cast<float>(ca)) * t;
			return static_cast<unsigned int>(v + 0.5f);
		};
	const unsigned int r = channel((a >> 16) & 0xff, (b >> 16) & 0xff);
	const unsigned int g = channel((a >> 8) & 0xff, (b >> 8) & 0xff);
	const unsigned int bl = channel(a & 0xff, b & 0xff);
	return (r << 16) | (g << 8) | bl;
}

void UIDraw::DrawGradientBar(int x, int y, int w, int h, float rate,
	unsigned int colLeft, unsigned int colRight)
{
	// 1ピクセル幅の縦線を並べて、中身の左端から右端へ色を変える
	const int fillW = static_cast<int>(w * std::clamp(rate, 0.0f, 1.0f));
	for (int i = 0; i < fillW; i++)
	{
		const float t = (fillW > 1) ? static_cast<float>(i) / (fillW - 1) : 0.0f;
		DrawBox(x + i, y, x + i + 1, y + h, LerpColor(colLeft, colRight, t), TRUE);
	}
}