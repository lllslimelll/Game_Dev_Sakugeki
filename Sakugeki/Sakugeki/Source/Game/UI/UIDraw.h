#pragma once

// UI の描画で共通に使う関数
namespace UIDraw
{
	// 不透明度を設定する（255 以上で通常の描画に戻す）
	void SetAlpha(int alpha);

	// 2色を補間する（色は 0xRRGGBB）
	unsigned int LerpColor(unsigned int a, unsigned int b, float t);

	// 左から右へのグラデーションで、幅 w * rate だけ塗る
	void DrawGradientBar(int x, int y, int w, int h, float rate,
		unsigned int colLeft, unsigned int colRight);
}