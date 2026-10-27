#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../Engine/Math/Vector2.h"

/// <summary>
/// 縦並びの選択メニュー（キー・パッド・マウス共通）
/// </summary>
class Menu
{
public:

	using Callback = std::function<void(void)>;

	// レイアウト設定
	struct Layout
	{
		Vector2 pos;		// 先頭項目の左上
		int itemWidth;		// 項目の幅（マウス判定・ハイライト用）
		int itemHeight;		// 項目の高さ
		int spacing;		// 項目間の余白
	};

	explicit Menu(const Layout& layout, int fontHandle = -1);
	~Menu(void) = default;

	// 項目の追加
	void AddItem(const std::wstring& label, Callback onDecide);

	// 入力処理（決定時はコールバックを呼ぶ）
	// ※コールバック内でシーン遷移しても安全なよう、呼び出しは関数の最後
	void Update(void);

	// 描画
	void Draw(void) const;

	// 選択位置
	int  GetSelectedIndex(void) const { return selected_; }
	void SetSelectedIndex(int index);

private:

	struct Item
	{
		std::wstring label;
		Callback onDecide;
	};

	Layout layout_;
	int fontHandle_;

	std::vector<Item> items_;
	int selected_;

	// 前フレームのマウス座標（マウスが動いた時だけ選択を奪う）
	Vector2 prevMousePos_;

	// 項目の上端Y
	int GetItemTop(int index) const;

	// マウス座標が乗っている項目（なければ -1）
	int HitTest(const Vector2& pos) const;
};