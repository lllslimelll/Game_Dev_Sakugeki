#include <DxLib.h>
#include "../../Engine/Input/Input.h"
#include "Menu.h"

Menu::Menu(const Layout& layout, int fontHandle)
	:
	layout_(layout),
	fontHandle_(fontHandle),
	items_(),
	selected_(0),
	prevMousePos_(Input::GetInstance().GetMousePos())
{
}

void Menu::AddItem(const std::wstring& label, Callback onDecide)
{
	items_.push_back({ label, std::move(onDecide) });
}

void Menu::SetSelectedIndex(int index)
{
	if (index < 0 || index >= static_cast<int>(items_.size())) return;
	selected_ = index;
}

void Menu::Update(void)
{
	if (items_.empty()) return;

	const auto& input = Input::GetInstance();
	const int num = static_cast<int>(items_.size());

	// キー・パッドでの選択（端でループ）
	if (input.IsTriggered(Input::Command::UP))
	{
		selected_ = (selected_ - 1 + num) % num;
	}
	if (input.IsTriggered(Input::Command::DOWN))
	{
		selected_ = (selected_ + 1) % num;
	}

	// マウスでの選択
	// 止まっているマウスがキー操作を上書きしないよう、動いた時だけ反映
	Vector2 mouse = input.GetMousePos();
	bool isMouseMoved =
		mouse.x != prevMousePos_.x || mouse.y != prevMousePos_.y;
	prevMousePos_ = mouse;

	int hit = HitTest(mouse);
	if (isMouseMoved && hit >= 0)
	{
		selected_ = hit;
	}

	// 決定判定
	bool isDecide = input.IsTriggered(Input::Command::DECIDE);
	if (input.IsTriggered(Input::Command::CLICK) && hit >= 0)
	{
		// 項目の上でクリックした時だけ決定
		selected_ = hit;
		isDecide = true;
	}

	// 決定処理
	if (isDecide && items_[selected_].onDecide)
	{
		items_[selected_].onDecide();
	}
}

void Menu::Draw(void) const
{
	for (int i = 0; i < static_cast<int>(items_.size()); i++)
	{
		int top = GetItemTop(i);
		bool isSelected = (i == selected_);

		// 選択中のハイライト
		if (isSelected)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
			DrawBox(layout_.pos.x, top,
				layout_.pos.x + layout_.itemWidth, top + layout_.itemHeight,
				0xffffff, true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// 文字（項目の高さの中央付近）
		unsigned int color = isSelected ? 0xffff00 : 0xffffff;
		int fontSize = (fontHandle_ != -1)
			? GetFontSizeToHandle(fontHandle_) : GetFontSize();
		int textY = top + (layout_.itemHeight - fontSize) / 2;
		int textX = layout_.pos.x + 16;

		if (fontHandle_ != -1)
		{
			DrawStringToHandle(textX, textY, items_[i].label.c_str(), color, fontHandle_);
		}
		else
		{
			DrawString(textX, textY, items_[i].label.c_str(), color);
		}
	}
}

int Menu::GetItemTop(int index) const
{
	return layout_.pos.y + index * (layout_.itemHeight + layout_.spacing);
}

int Menu::HitTest(const Vector2& pos) const
{
	if (pos.x < layout_.pos.x || pos.x >= layout_.pos.x + layout_.itemWidth) return -1;

	for (int i = 0; i < static_cast<int>(items_.size()); i++)
	{
		int top = GetItemTop(i);
		if (pos.y >= top && pos.y < top + layout_.itemHeight) return i;
	}
	return -1;
}