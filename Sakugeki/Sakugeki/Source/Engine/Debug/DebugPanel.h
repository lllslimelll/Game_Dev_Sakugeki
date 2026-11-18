#pragma once

// デバッグ画面の、ウィンドウ1つ分の中身
// DebugSystem が毎フレーム、ImGui::Begin と End の間で Draw を呼ぶ（一時停止中も呼ばれる）
class DebugPanel
{
public:

	virtual ~DebugPanel(void) = default;

	// ウィンドウのタイトル
	// ImGui は名前でウィンドウを見分けるので、同じ名前のパネルを2つ作らない
	virtual const char* GetName(void) const = 0;

	// 中身（ここで ImGui:: を直接呼ぶ。Begin と End は DebugSystem が呼ぶ）
	virtual void Draw(void) = 0;
};