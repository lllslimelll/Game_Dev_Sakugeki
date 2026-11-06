#include <algorithm>
#include <cmath>
#include <string>
#include <DxLib.h>
#include "DebugHud.h"

namespace
{
	constexpr float PI = 3.14159265f;
	constexpr float TAU = PI * 2.0f;

	float Lerp(float a, float b, float t) { return a + (b - a) * t; }

	// ゆっくり始まりゆっくり終わる補間
	float Smooth(float t) { return t * t * (3.0f - 2.0f * t); }

	float Rate(float value, float maxValue)
	{
		if (maxValue <= 0.0f) { return 0.0f; }
		return std::clamp(value / maxValue, 0.0f, 1.0f);
	}
}

DebugHud::DebugHud(void)
	:
	fontSmall_(-1),
	fontGuide_(-1),
	fontMove_(-1),
	fontNum_(-1),
	time_(0.0f),
	lagHp_(0.0f),
	lagWait_(0.0f),
	lastHp_(-1)
{
}

DebugHud::~DebugHud(void)
{
}

void DebugHud::Init(void)
{
	fontSmall_ = CreateFontToHandle(nullptr, 11, 3, DX_FONTTYPE_ANTIALIASING_4X4);
	fontGuide_ = CreateFontToHandle(nullptr, 13, 6, DX_FONTTYPE_ANTIALIASING_4X4);
	fontMove_ = CreateFontToHandle(nullptr, 14, 7, DX_FONTTYPE_ANTIALIASING_4X4);
	fontNum_ = CreateFontToHandle(nullptr, 19, 9, DX_FONTTYPE_ANTIALIASING_4X4);
}

void DebugHud::Update(const HudData& data, float deltaTime)
{
	time_ += deltaTime;

	// ---- 遅れて減る HPGauge ----
	if (lastHp_ < 0)
	{
		// 最初の1回
		lagHp_ = static_cast<float>(data.hp);
	}
	else if (data.hp < lastHp_)
	{
		// 減った瞬間は少し待ってから追いかける
		lagWait_ = LAG_DELAY;
	}
	lastHp_ = data.hp;

	if (lagHp_ < data.hp)
	{
		// 回復はすぐ追いつく
		lagHp_ = static_cast<float>(data.hp);
	}
	else if (lagWait_ > 0.0f)
	{
		lagWait_ -= deltaTime;
	}
	else
	{
		lagHp_ += (data.hp - lagHp_) * (std::min)(1.0f, LAG_SPEED * deltaTime);
	}

	// ---- 技を達成したときの点灯 ----
	if (prevDone_.size() != data.moves.size())
	{
		// リストが作り直されたら、今の状態から始める（点灯させない）
		prevDone_.clear();
		for (const auto& move : data.moves) { prevDone_.emplace_back(move.isDone); }
		flash_.assign(data.moves.size(), 0.0f);
	}
	for (size_t i = 0; i < data.moves.size(); i++)
	{
		if (!prevDone_[i] && data.moves[i].isDone)
		{
			flash_[i] = FLASH_TIME;
		}
		flash_[i] = (std::max)(0.0f, flash_[i] - deltaTime);
		prevDone_[i] = data.moves[i].isDone;
	}
}

void DebugHud::Draw(const HudData& data) const
{
	int screenW = 0, screenH = 0;
	GetDrawScreenSize(&screenW, &screenH);

	DrawSpirit(data);
	DrawMoves(data, screenW);
	DrawBud(data, screenW, screenH);

	SetAlpha(255);
}

void DebugHud::Release(void)
{
	for (int* font : { &fontSmall_, &fontGuide_, &fontMove_, &fontNum_ })
	{
		if (*font != -1)
		{
			DeleteFontToHandle(*font);
			*font = -1;
		}
	}
}

void DebugHud::DrawHp(const HudData& data) const
{
	const float rate = Rate(static_cast<float>(data.hp), static_cast<float>(data.maxHp));
	const float lagRate = Rate(lagHp_, static_cast<float>(data.maxHp));
	const bool isLow = rate < LOW_HPGauge_RATE;

	const int barX = MARGIN_X + HPGauge_ICON_R * 2 + 10;
	const int barY = MARGIN_TOP + HPGauge_BAR_TOP;

	// ---- ハートのアイコン（ゲージの下端より少し下まで出す） ----
	const float iconX = static_cast<float>(MARGIN_X + HPGauge_ICON_R);
	const float iconY = static_cast<float>(barY + HPGauge_BAR_H + 12 - HPGauge_ICON_R);
	SetAlpha(184);
	DrawCircleAA(iconX, iconY, HPGauge_ICON_R, 32, COL_PANEL, TRUE);
	SetAlpha(90);
	DrawCircleAA(iconX, iconY, HPGauge_ICON_R, 32, COL_INK, FALSE, 1.5f);
	SetAlpha(255);
	DrawCircleAA(iconX - 4.5f, iconY - 2.5f, 5.5f, 16, 0xff6f91, TRUE);
	DrawCircleAA(iconX + 4.5f, iconY - 2.5f, 5.5f, 16, 0xff6f91, TRUE);
	DrawTriangleAA(iconX - 10.0f, iconY - 0.5f, iconX + 10.0f, iconY - 0.5f, iconX, iconY + 9.5f, 0xff6f91, TRUE);

	// ---- ゲージ ----
	// 背景と枠
	SetAlpha(115);
	DrawBox(barX, barY, barX + HPGauge_BAR_W, barY + HPGauge_BAR_H, 0x000000, TRUE);
	// 遅れて減る部分
	SetAlpha(140);
	DrawBox(barX, barY, barX + static_cast<int>(HPGauge_BAR_W * lagRate), barY + HPGauge_BAR_H, COL_PETAL, TRUE);
	SetAlpha(255);
	// 現在の HPGauge
	if (isLow)
	{
		DrawGauge(barX, barY, HPGauge_BAR_W, HPGauge_BAR_H, rate, 0xa3362a, COL_EMBER);
	}
	else
	{
		DrawGauge(barX, barY, HPGauge_BAR_W, HPGauge_BAR_H, rate, 0x4f9a4a, COL_LEAF);
	}
	SetAlpha(64);
	DrawBox(barX, barY, barX + HPGauge_BAR_W, barY + HPGauge_BAR_H, COL_INK, FALSE);
	SetAlpha(255);

	// ---- 吹き出し（先端がゲージの末端を指す） ----
	const std::wstring num = std::to_wstring(data.hp);
	const int numW = GetDrawStringWidthToHandle(num.c_str(), static_cast<int>(num.size()), fontNum_);
	const int numH = GetFontSizeToHandle(fontNum_);
	const int bubbleW = numW + 18;
	const int bubbleH = numH + 7;
	const int tipX = barX + static_cast<int>(HPGauge_BAR_W * rate);
	const int bubbleX = tipX - bubbleW / 2;
	const int bubbleY = MARGIN_TOP;
	const unsigned int bubbleCol = isLow ? COL_EMBER : COL_INK;
	const unsigned int numCol = isLow ? 0x2a0d07 : 0x13201a;

	DrawRoundRect(bubbleX, bubbleY, bubbleX + bubbleW, bubbleY + bubbleH, 9, 9, bubbleCol, TRUE);
	DrawTriangleAA(
		static_cast<float>(tipX - 6), static_cast<float>(bubbleY + bubbleH),
		static_cast<float>(tipX + 6), static_cast<float>(bubbleY + bubbleH),
		static_cast<float>(tipX), static_cast<float>(bubbleY + bubbleH + 6),
		bubbleCol, TRUE);
	DrawStringToHandle(bubbleX + 9, bubbleY + 3, num.c_str(), numCol, fontNum_);
}

void DebugHud::DrawSpirit(const HudData& data) const
{
	const float rate = Rate(data.spirit, data.maxSpirit);
	const bool isFull = rate >= 1.0f;

	// HPGauge の行の下
	const int rowTop = MARGIN_TOP + HPGauge_BAR_TOP + HPGauge_BAR_H + 12 + 8;
	const int centerY = rowTop + SP_ICON_R;

	// ---- 精霊のアイコン ----
	const float iconX = static_cast<float>(MARGIN_X + 5 + SP_ICON_R);
	const float iconY = static_cast<float>(centerY);
	SetAlpha(184);
	DrawCircleAA(iconX, iconY, SP_ICON_R, 32, COL_PANEL, TRUE);
	SetAlpha(90);
	DrawCircleAA(iconX, iconY, SP_ICON_R, 32, COL_INK, FALSE, 1.5f);
	SetAlpha(56);
	DrawCircleAA(iconX, iconY, 10.0f, 24, 0x9cf08a, TRUE);
	SetAlpha(115);
	DrawCircleAA(iconX, iconY, 6.5f, 24, 0x9cf08a, TRUE);
	SetAlpha(255);
	DrawCircleAA(iconX, iconY, 4.0f, 16, 0xe9ffe3, TRUE);

	// ---- ゲージ ----
	const int barX = MARGIN_X + 5 + SP_ICON_R * 2 + 10;
	const int barY = centerY - SP_BAR_H / 2;

	if (isFull)
	{
		// 満タンは光らせる
		const float pulse = 0.5f + 0.5f * std::sin(time_ * 4.0f);
		SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>(60 + 60 * pulse));
		DrawRoundRect(barX - 4, barY - 4, barX + SP_BAR_W + 4, barY + SP_BAR_H + 4, 6, 6, COL_PETAL, TRUE);
	}
	SetAlpha(115);
	DrawBox(barX, barY, barX + SP_BAR_W, barY + SP_BAR_H, 0x000000, TRUE);
	SetAlpha(255);
	DrawGauge(barX, barY, SP_BAR_W, SP_BAR_H, rate, 0xe9d7a4, COL_PETAL);
	SetAlpha(isFull ? 255 : 64);
	DrawBox(barX, barY, barX + SP_BAR_W, barY + SP_BAR_H, isFull ? COL_PETAL : COL_INK, FALSE);
	SetAlpha(255);

	// ---- ラベル ----
	const std::wstring label = L"精霊の加護";
	const int labelX = barX + SP_BAR_W + 10;
	const int smallH = GetFontSizeToHandle(fontSmall_);
	DrawStringToHandle(labelX, centerY - smallH / 2, label.c_str(), COL_DIM, fontSmall_);

	// ---- 満タンのときのガイド ----
	if (data.guide == HudData::GUIDE::NONE) { return; }

	const int guideX = labelX
		+ GetDrawStringWidthToHandle(label.c_str(), static_cast<int>(label.size()), fontSmall_) + 10;

	std::vector<Piece> pieces;
	unsigned int color = COL_PETAL;
	if (data.guide == HudData::GUIDE::HEAL)
	{
		pieces = { { PIECE::KEY, L"↓" }, { PIECE::TEXT, L"回復" } };
	}
	else
	{
		pieces = { { PIECE::KEY, L"Y" }, { PIECE::PLUS, L"+" }, { PIECE::KEY, L"B" }, { PIECE::TEXT, L"フィニッシュ" } };
		color = 0xffffff;
	}
	DrawPieces(guideX, centerY, pieces, GUIDE_KEY_H, fontGuide_, color, true);
}

void DebugHud::DrawMoves(const HudData& data, int screenW) const
{
	const int right = screenW - MARGIN_X;
	int y = MARGIN_TOP + 4;

	// ---- 見出し ----
	const std::wstring head = L"技リスト";
	const int headW = GetDrawStringWidthToHandle(head.c_str(), static_cast<int>(head.size()), fontSmall_);
	DrawStringToHandle(right - headW, y, head.c_str(), COL_DIM, fontSmall_);
	y += GetFontSizeToHandle(fontSmall_) + 2 + MOVE_ROW_GAP;

	// ---- 各項目（右揃え） ----
	const int checkW = 14;
	for (size_t i = 0; i < data.moves.size(); i++)
	{
		const HudMove& move = data.moves[i];
		const std::vector<Piece> pieces = ParseLabel(move.label);

		const int centerY = y + MOVE_ROW_H / 2;
		const int contentW = checkW + 4 + DrawPieces(0, 0, pieces, KEY_H, fontMove_, COL_INK, false);
		const int rowW = 8 + contentW + 10 + 2;
		const int x1 = right - rowW;
		const int x2 = right;

		// 達成済みは薄くする
		const int rowAlpha = move.isDone ? 102 : 255;

		// 背景（達成した瞬間は桜色に光って戻る）
		const float flash = (i < flash_.size()) ? flash_[i] / FLASH_TIME : 0.0f;
		SetAlpha(static_cast<int>(Lerp(140.0f, 178.0f, flash) * rowAlpha / 255));
		DrawBox(x1, y, x2, y + MOVE_ROW_H, LerpColor(COL_PANEL, COL_SAKURA, flash), TRUE);

		// 右端の線（達成済みは消す）
		if (!move.isDone)
		{
			SetAlpha(255);
			DrawBox(x2 - 2, y, x2, y + MOVE_ROW_H, COL_SAKURA, TRUE);
		}

		// チェック
		SetAlpha(rowAlpha);
		const int contentX = x1 + 8;
		if (move.isDone)
		{
			DrawStringToHandle(contentX + 1, centerY - GetFontSizeToHandle(fontMove_) / 2, L"✓", COL_SAKURA, fontMove_);
		}

		// 技のラベル
		DrawPieces(contentX + checkW + 4, centerY, pieces, KEY_H, fontMove_, COL_INK, true);

		// 打ち消し線
		if (move.isDone)
		{
			DrawLineAA(
				static_cast<float>(contentX + checkW + 2), static_cast<float>(centerY),
				static_cast<float>(contentX + contentW + 2), static_cast<float>(centerY),
				COL_SAKURA, 1.5f);
		}

		SetAlpha(255);
		y += MOVE_ROW_H + MOVE_ROW_GAP;
	}
}

void DebugHud::DrawBud(const HudData& data, int screenW, int screenH) const
{
	const int count = static_cast<int>(data.moves.size());
	const int stage = static_cast<int>(std::count_if(data.moves.begin(), data.moves.end(),
		[](const HudMove& m) { return m.isDone; }));
	const float open = (count > 0) ? static_cast<float>(stage) / count : 0.0f;
	const bool isBloom = data.isBloom;

	// ---- 配置（右下。蕾の下にラベル） ----
	const int labelH = GetFontSizeToHandle(fontGuide_);
	const int labelY = screenH - MARGIN_BOTTOM - labelH;
	const float cx = static_cast<float>(screenW - MARGIN_X - BUD_SIZE / 2);
	const float cy = static_cast<float>(labelY - 4 - BUD_SIZE / 2);
	const float R = BUD_SIZE * 0.46f;
	const float pulse = std::sin(time_ * 4.0f);

	// ---- 背景の円 ----
	SetAlpha(235);
	DrawCircleAA(cx, cy, R, 48, 0x1c3027, TRUE);
	if (isBloom)
	{
		SetAlpha(static_cast<int>((0.7f + 0.3f * pulse) * 255));
		DrawCircleAA(cx, cy, R, 48, COL_SAKURA, FALSE, 2.5f);
	}
	else
	{
		SetAlpha(77);
		DrawCircleAA(cx, cy, R, 48, COL_INK, FALSE, 2.0f);
	}
	SetAlpha(255);

	// 花の中心（開くほど少し上がる）
	const float fx = cx;
	const float fy = cy - R * 0.12f + R * 0.1f * (1.0f - open);

	// ---- 茎と葉 ----
	const float midX = cx + R * 0.05f;
	const float midY = cy + R * 0.4f;
	DrawLineAA(cx + R * 0.15f, cy + R * 0.92f, midX, midY, 0x5d9b4c, 3.5f);
	DrawLineAA(midX, midY, fx, fy, 0x5d9b4c, 3.5f);
	DrawPetal(cx + R * 0.12f, cy + R * 0.55f, 1.07f, R * 0.38f, R * 0.13f, 0x6fb85a);
	DrawPetal(cx + R * 0.05f, cy + R * 0.40f, -1.0f, R * 0.32f, R * 0.11f, 0x6fb85a);

	// ---- 開花したら光る ----
	if (isBloom)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, static_cast<int>((0.45f + 0.15f * pulse) * 120));
		DrawCircleAA(fx, fy, R * 0.85f, 48, COL_PETAL, TRUE);
		SetAlpha(255);
	}

	// ---- 花びら5枚：閉じた蕾 → 開いた桜（72°間隔） ----
	const int order[] = { 0, 4, 1, 3, 2 };
	const float smoothOpen = Smooth(open);
	for (int i : order)
	{
		const float angle = Lerp((i - 2) * 0.15f, (i - 2) * TAU / 5.0f, smoothOpen);
		const float length = R * Lerp(0.36f, 0.5f, open);
		const float width = R * Lerp(0.13f, 0.27f, open);
		const unsigned int color = LerpColor(
			LerpColor(0xc2386e, 0xf08bb2, open),
			LerpColor(0xe06a98, 0xffe9f1, open), 0.5f);
		DrawPetal(fx, fy, angle, length, width, color);
	}

	// ---- 花芯（開いてきたら出す） ----
	if (open > 0.6f)
	{
		SetAlpha(static_cast<int>((open - 0.6f) / 0.4f * 255));
		for (int k = 0; k < 8; k++)
		{
			const float a = k * TAU / 8.0f + 0.2f;
			const float r = R * 0.13f;
			const float px = fx + std::sin(a) * r;
			const float py = fy - std::cos(a) * r;
			DrawLineAA(fx, fy, px, py, 0xf6d9a6, 2.0f);
			DrawCircleAA(px, py, BUD_SIZE * 0.012f, 8, 0xf2c53d, TRUE);
		}
		SetAlpha(255);
	}

	// ---- がく（閉じているときだけ） ----
	if (open < 0.7f)
	{
		SetAlpha(static_cast<int>((1.0f - open / 0.7f) * 255));
		for (int s = -1; s <= 1; s++)
		{
			DrawPetal(fx, fy + R * 0.02f, s * 0.5f, R * 0.2f, R * 0.06f, 0x4f8d40);
		}
		SetAlpha(255);
	}

	// ---- ラベル ----
	const std::wstring label = isBloom
		? L"必殺技 L3+R3"
		: std::to_wstring(stage) + L" / " + std::to_wstring(count);
	const int labelW = GetDrawStringWidthToHandle(label.c_str(), static_cast<int>(label.size()), fontGuide_);
	DrawStringToHandle(static_cast<int>(cx) - labelW / 2, labelY, label.c_str(),
		isBloom ? COL_PETAL : COL_DIM, fontGuide_);
}

void DebugHud::DrawGauge(int x, int y, int w, int h, float rate,
	unsigned int colLeft, unsigned int colRight) const
{
	// 中身の左端から右端への色のグラデーション（1ピクセル幅の縦線を並べる）
	const int fillW = static_cast<int>(w * rate);
	for (int i = 0; i < fillW; i++)
	{
		const float t = (fillW > 1) ? static_cast<float>(i) / (fillW - 1) : 0.0f;
		DrawBox(x + i, y, x + i + 1, y + h, LerpColor(colLeft, colRight, t), TRUE);
	}
}

void DebugHud::DrawPetal(float x, float y, float angle, float length, float width, unsigned int color) const
{
	// 根元 (0, 0) から先端 (0, -length) に向かう菱形を、angle（上が 0、時計回り）だけ回す
	const float c = std::cos(angle);
	const float s = std::sin(angle);
	auto toWorld = [&](float lx, float ly, float& wx, float& wy)
		{
			wx = x + lx * c - ly * s;
			wy = y + lx * s + ly * c;
		};

	float bx, by, lx, ly, rx, ry, tx, ty, mx, my;
	toWorld(0.0f, 0.0f, bx, by);
	toWorld(-width, -length * 0.45f, lx, ly);
	toWorld(width, -length * 0.45f, rx, ry);
	toWorld(0.0f, -length, tx, ty);
	toWorld(0.0f, -length * 0.55f, mx, my);

	DrawTriangleAA(bx, by, lx, ly, tx, ty, color, TRUE);
	DrawTriangleAA(bx, by, rx, ry, tx, ty, color, TRUE);
	// 先を丸くする
	DrawCircleAA(mx, my, width * 0.9f, 16, color, TRUE);
}

int DebugHud::DrawKey(int x, int centerY, const std::wstring& key, int height, int font, bool isDraw) const
{
	const int textW = GetDrawStringWidthToHandle(key.c_str(), static_cast<int>(key.size()), font);
	const int w = (std::max)(height, textW + 8);
	if (!isDraw) { return w; }

	// ボタンごとの色
	unsigned int bg = COL_INK;
	unsigned int fg = COL_KEY_DARK;
	int radius = height / 2;
	if (key == L"X") { bg = COL_KEY_X; fg = 0xffffff; }
	else if (key == L"Y") { bg = COL_KEY_Y; }
	else if (key == L"A") { bg = COL_KEY_A; fg = 0xffffff; }
	else if (key == L"B") { bg = COL_KEY_B; fg = 0xffffff; }
	else if (key == L"LT" || key == L"RT") { bg = COL_KEY_TRIGGER; radius = 4; }

	const int y1 = centerY - height / 2;
	DrawRoundRect(x, y1, x + w, y1 + height, radius, radius, bg, TRUE);
	DrawStringToHandle(x + (w - textW) / 2, centerY - GetFontSizeToHandle(font) / 2, key.c_str(), fg, font);
	return w;
}

int DebugHud::DrawPieces(int x, int centerY, const std::vector<Piece>& pieces,
	int keyH, int font, unsigned int textColor, bool isDraw) const
{
	constexpr int GAP = 3;
	int cursor = x;

	for (size_t i = 0; i < pieces.size(); i++)
	{
		const Piece& piece = pieces[i];
		if (i > 0) { cursor += GAP; }

		switch (piece.type)
		{
		case PIECE::KEY:
			cursor += DrawKey(cursor, centerY, piece.text, keyH, font, isDraw);
			break;

		case PIECE::PLUS:
		case PIECE::TEXT:
		{
			const int useFont = (piece.type == PIECE::PLUS) ? fontSmall_ : font;
			const unsigned int color = (piece.type == PIECE::PLUS) ? COL_DIM : textColor;
			const int w = GetDrawStringWidthToHandle(piece.text.c_str(), static_cast<int>(piece.text.size()), useFont);
			if (isDraw)
			{
				DrawStringToHandle(cursor, centerY - GetFontSizeToHandle(useFont) / 2, piece.text.c_str(), color, useFont);
			}
			cursor += w;
			break;
		}
		}
	}

	return cursor - x;
}

std::vector<DebugHud::Piece> DebugHud::ParseLabel(const std::wstring& label)
{
	std::vector<Piece> ret;
	std::wstring text;

	// 溜めた普通の文字を部品にする
	auto flushText = [&]()
		{
			if (text.empty()) { return; }
			ret.push_back({ PIECE::TEXT, text });
			text.clear();
		};

	size_t i = 0;
	while (i < label.size())
	{
		if (label[i] == L'+')
		{
			flushText();
			ret.push_back({ PIECE::PLUS, L"+" });
			i++;
		}
		else if (label.compare(i, 2, L"LT") == 0 || label.compare(i, 2, L"RT") == 0)
		{
			flushText();
			ret.push_back({ PIECE::KEY, label.substr(i, 2) });
			i += 2;
		}
		else if (std::wstring(L"XYAB").find(label[i]) != std::wstring::npos)
		{
			flushText();
			ret.push_back({ PIECE::KEY, std::wstring(1, label[i]) });
			i++;
		}
		else
		{
			text += label[i];
			i++;
		}
	}
	flushText();

	return ret;
}

unsigned int DebugHud::LerpColor(unsigned int a, unsigned int b, float t)
{
	auto channel = [t](unsigned int ca, unsigned int cb)
		{
			return static_cast<unsigned int>(Lerp(static_cast<float>(ca), static_cast<float>(cb), t) + 0.5f);
		};
	const unsigned int r = channel((a >> 16) & 0xff, (b >> 16) & 0xff);
	const unsigned int g = channel((a >> 8) & 0xff, (b >> 8) & 0xff);
	const unsigned int bl = channel(a & 0xff, b & 0xff);
	return (r << 16) | (g << 8) | bl;
}

void DebugHud::SetAlpha(int alpha)
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