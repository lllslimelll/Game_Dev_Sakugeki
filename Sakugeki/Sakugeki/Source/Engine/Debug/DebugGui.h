#pragma once

// ImGui を DxLib の上で動かすクラス
// 初期化と後始末、フレームの開始と描画だけを行う
class DebugGui
{
public:

	DebugGui(void);
	~DebugGui(void);

	// ImGui のコンテキストは1つだけなのでコピー禁止
	DebugGui(const DebugGui&) = delete;
	DebugGui& operator=(const DebugGui&) = delete;

	// ImGui を作り、DxLib のウィンドウと D3D11 デバイスにつなぐ。失敗したら false
	bool Init(void);

	// デバッグ画面を開いているか（マウスカーソルを ImGui が扱うかどうかが変わる）
	void SetActive(bool isActive);

	// フレームの開始（ここから EndFrame までの間だけ ImGui:: を呼べる）
	void BeginFrame(void);

	// ImGui を描き、DxLib の描画設定を元に戻す
	void EndFrame(void);

	// ImGui の後始末
	void Release(void);

	// ImGui がマウスを使っているか（ウィンドウの上にカーソルがある、ドラッグ中など）
	bool IsUsingMouse(void) const;

private:

	bool isInitialized_;

	// 日本語が出るフォントを読む（なければ ImGui の標準フォント）
	void LoadFont(void);

	// マウスの位置とボタンを ImGui に渡す
	void UpdateMouse(void);
};