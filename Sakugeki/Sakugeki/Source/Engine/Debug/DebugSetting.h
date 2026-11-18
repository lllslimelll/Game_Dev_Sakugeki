#pragma once

// デバッグ機能の状態
struct DebugSetting
{
	bool isVisible = false;			// デバッグ画面（F3）
	bool isPaused = false;			// 一時停止（F5）
	bool isStepRequested = false;	// 1フレームだけ進める要求（F6。DebugSystem が使ったら false に戻す）
	bool showDemoWindow = false;	
};