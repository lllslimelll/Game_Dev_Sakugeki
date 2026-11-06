#include <algorithm>
#include <string>
#include <DxLib.h>
#include "../../../Engine/Component/Health.h"
#include "../UIDraw.h"
#include "HPGauge.h"

namespace
{
	// 割合（最大値が 0 以下なら 0）
	float Rate(float value, float maxValue)
	{
		if (maxValue <= 0.0f) { return 0.0f; }
		return std::clamp(value / maxValue, 0.0f, 1.0f);
	}
}

HPGauge::HPGauge(const Health& health)
	:
	health_(health),
	font_(-1),
	lagHp_(0.0f),
	lagWait_(0.0f),
	lastHp_(0)
{
}

HPGauge::~HPGauge(void)
{
}

void HPGauge::Init(void)
{
	font_ = CreateFontToHandle(nullptr, NUM_FONT_SIZE, 9, DX_FONTTYPE_ANTIALIASING_4X4);

	// 遅れて減る部分を、今の HPGauge に揃える
	lastHp_ = health_.GetHp();
	lagHp_ = static_cast<float>(lastHp_);
	lagWait_ = 0.0f;
}

void HPGauge::Update(float deltaTime)
{
	const int hp = health_.GetHp();

	// 減った瞬間は、少し待ってから追いかける
	if (hp < lastHp_)
	{
		lagWait_ = LAG_DELAY;
	}
	lastHp_ = hp;

	if (lagHp_ < hp)
	{
		// 回復はすぐ追いつく
		lagHp_ = static_cast<float>(hp);
	}
	else if (lagWait_ > 0.0f)
	{
		lagWait_ -= deltaTime;
	}
	else
	{
		lagHp_ += (hp - lagHp_) * (std::min)(1.0f, LAG_SPEED * deltaTime);
	}
}

void HPGauge::Draw(void) const
{
	const int hp = health_.GetHp();
	const float maxHp = static_cast<float>(health_.GetMaxHp());
	const float rate = Rate(static_cast<float>(hp), maxHp);
	const float lagRate = Rate(lagHp_, maxHp);
	const bool isLow = rate < LOW_RATE;

	const int barX = POS_X + ICON_R * 2 + ICON_GAP;
	const int barY = POS_Y + BAR_TOP;

	DrawIcon(barY);
	DrawBar(barX, barY, rate, lagRate, isLow);
	DrawBubble(barX + static_cast<int>(BAR_W * rate), hp, isLow);

	UIDraw::SetAlpha(255);
}

void HPGauge::Release(void)
{
	if (font_ != -1)
	{
		DeleteFontToHandle(font_);
		font_ = -1;
	}
}

void HPGauge::DrawIcon(int barY) const
{
	// ゲージの下端より少し下まで出す
	const float x = static_cast<float>(POS_X + ICON_R);
	const float y = static_cast<float>(barY + BAR_H + 12 - ICON_R);

	// 背景の円と枠
	UIDraw::SetAlpha(184);
	DrawCircleAA(x, y, ICON_R, 32, COL_PANEL, TRUE);
	UIDraw::SetAlpha(90);
	DrawCircleAA(x, y, ICON_R, 32, COL_INK, FALSE, 1.5f);
	UIDraw::SetAlpha(255);

	// ハート（円2つと三角形）
	DrawCircleAA(x - 4.5f, y - 2.5f, 5.5f, 16, COL_HEART, TRUE);
	DrawCircleAA(x + 4.5f, y - 2.5f, 5.5f, 16, COL_HEART, TRUE);
	DrawTriangleAA(x - 10.0f, y - 0.5f, x + 10.0f, y - 0.5f, x, y + 9.5f, COL_HEART, TRUE);
}

void HPGauge::DrawBar(int barX, int barY, float rate, float lagRate, bool isLow) const
{
	// 背景
	UIDraw::SetAlpha(115);
	DrawBox(barX, barY, barX + BAR_W, barY + BAR_H, 0x000000, TRUE);

	// 遅れて減る部分
	UIDraw::SetAlpha(140);
	DrawBox(barX, barY, barX + static_cast<int>(BAR_W * lagRate), barY + BAR_H, COL_PETAL, TRUE);
	UIDraw::SetAlpha(255);

	// 現在の HPGauge
	if (isLow)
	{
		UIDraw::DrawGradientBar(barX, barY, BAR_W, BAR_H, rate, COL_EMBER_DARK, COL_EMBER);
	}
	else
	{
		UIDraw::DrawGradientBar(barX, barY, BAR_W, BAR_H, rate, COL_LEAF_DARK, COL_LEAF);
	}

	// 枠
	UIDraw::SetAlpha(64);
	DrawBox(barX, barY, barX + BAR_W, barY + BAR_H, COL_INK, FALSE);
	UIDraw::SetAlpha(255);
}

void HPGauge::DrawBubble(int tipX, int hp, bool isLow) const
{
	const std::wstring num = std::to_wstring(hp);
	const int numW = GetDrawStringWidthToHandle(num.c_str(), static_cast<int>(num.size()), font_);
	const int numH = GetFontSizeToHandle(font_);

	const int w = numW + BUBBLE_PAD_X * 2;
	const int h = numH + 7;
	const int x = tipX - w / 2;
	const int y = POS_Y;

	const unsigned int bgColor = isLow ? COL_EMBER : COL_INK;
	const unsigned int numColor = isLow ? COL_NUM_LOW : COL_NUM;

	// 本体
	DrawRoundRect(x, y, x + w, y + h, BUBBLE_R, BUBBLE_R, bgColor, TRUE);

	// 先端（下向きの三角形）
	DrawTriangleAA(
		static_cast<float>(tipX - BUBBLE_TIP), static_cast<float>(y + h),
		static_cast<float>(tipX + BUBBLE_TIP), static_cast<float>(y + h),
		static_cast<float>(tipX), static_cast<float>(y + h + BUBBLE_TIP),
		bgColor, TRUE);

	// 数値
	DrawStringToHandle(x + BUBBLE_PAD_X, y + 3, num.c_str(), numColor, font_);
}