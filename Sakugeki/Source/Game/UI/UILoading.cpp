#include <DxLib.h>
#include "../../Engine/Core/Application.h"
#include "UILoading.h"

namespace
{
	// ゲージ（仮）
	constexpr int GAUGE_X = 100;
	constexpr int GAUGE_Y = 140;
	constexpr int GAUGE_W = 300;
	constexpr int GAUGE_H = 20;

	// 文字（仮）
	constexpr int TEXT_X = 100;
	constexpr int TEXT_Y = 100;
}

void UILoading::Load(void)
{
	// 画像を使う段階でここに読み込みを書く
}

void UILoading::Draw(float progress, float time) const
{
	// 背景
	DrawBox(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, 0x000000, true);

	// 文字（点が増えるアニメーション）
	static const wchar_t* TEXTS[] = { L"NOW LOADING", L"NOW LOADING.", L"NOW LOADING..", L"NOW LOADING..." };
	int dot = static_cast<int>(time * 3.0f) % 4;
	DrawString(TEXT_X, TEXT_Y, TEXTS[dot], 0xffffff);

	// ゲージ
	DrawBox(GAUGE_X, GAUGE_Y, GAUGE_X + GAUGE_W, GAUGE_Y + GAUGE_H, 0xffffff, false);
	DrawBox(GAUGE_X, GAUGE_Y,
		GAUGE_X + static_cast<int>(GAUGE_W * progress), GAUGE_Y + GAUGE_H,
		0xffffff, true);
}