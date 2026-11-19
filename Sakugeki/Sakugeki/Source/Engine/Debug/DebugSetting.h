#pragma once
#include <cstdint>

// デバッグ機能の状態
struct DebugSetting
{
	bool isVisible = false;			// デバッグ画面（F3）
	bool isPaused = false;			// 一時停止（F5）
	bool isStepRequested = false;	// 1フレームだけ進める要求（F6。DebugSystem が使ったら false に戻す）
	bool showDemoWindow = false;	// ImGui のデモウィンドウ

	// 3D 空間の表示（World パネル）
	bool showOriginAxes = true;					// 原点の XYZ 軸
	bool showGrid = false;						// グリッド
	float gridHeight = 0.0f;					// グリッドを描く高さ
	bool showRays = false;						// Linecast
	bool showColliders = false;					// コライダー
	std::uint32_t colliderLayers = 0xFFFFFFFFu;	// 表示する Layer（ビットごと）
	bool drawOnTop = false;						// 前面に表示（壁の後ろも見せる）
};