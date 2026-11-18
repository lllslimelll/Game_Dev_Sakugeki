#pragma once
#include <array>
#include <memory>
#include <vector>
#include "DebugSetting.h"
#include "DebugPanel.h"
#include "FrameStats.h"

class DebugGui;

// デバッグシステム
// 部品を、毎フレーム決まった順番で動かす
class DebugSystem
{
public:

	// targetFps : 目標の FPS（グラフの目安）
	explicit DebugSystem(int targetFps);
	~DebugSystem(void);

	// ImGui を準備し、標準のパネルを登録する
	void Init(void);

	// フレームの最初（Input の更新の後）
	void BeginFrame(void);

	// このフレームでゲームを進めるか（一時停止中は false。コマ送りのときだけ true）
	bool ShouldUpdateGame(void) const;

	// ゲームの更新の後
	void EndUpdate(void);

	// シーンの描画の後、ScreenFlip の前
	void Draw(void);

	//
	void EndFrame(void);

	// 解放
	void Release(void);

	// パネルを足す（DebugSystem が所有する）
	void AddPanel(std::unique_ptr<DebugPanel> panel);

	const DebugSetting& GetSettings(void) const;

	// デバッグ画面がマウスを使っているか
	// true の間、ゲーム側はマウスを使わない（カメラがカーソルを中心に戻さない、など）
	bool IsUsingMouse(void) const;

private:

	// パネルと、そのウィンドウを開いているか
	struct PanelEntry
	{
		std::unique_ptr<DebugPanel> panel;
		bool isOpen = true;
	};

	// キーの数（DxLib の GetHitKeyStateAll が使う配列の大きさ）
	static constexpr int KEY_NUM = 256;

	DebugSetting setting_;
	FrameStats frameStats_;
	std::unique_ptr<DebugGui> gui_;
	std::vector<PanelEntry> panels_;

	float targetFrameMs_;

	// キーの状態（今と前のフレーム）
	std::array<char, KEY_NUM> keyState_;
	std::array<char, KEY_NUM> prevKeyState_;

	void UpdateKeys(void);
	bool IsKeyTriggered(int key) const;

	// デバッグ画面の表示を切り替える
	void SetVisible(bool isVisible);

	// メニューバーと各パネルを描く
	void DrawGui(void);
};