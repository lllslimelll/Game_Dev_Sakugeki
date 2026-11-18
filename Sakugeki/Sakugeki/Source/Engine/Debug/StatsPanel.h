#pragma once
#include "DebugPanel.h"

class FrameStats;
struct DebugSetting;

// 処理時間と、一時停止・コマ送りのパネル
class StatsPanel : public DebugPanel
{
public:

	// targetFrameMs : 目標の1フレームの時間（グラフの目安）
	StatsPanel(const FrameStats& stats, DebugSetting& settings, float targetFrameMs);
	~StatsPanel(void) override;

	const char* GetName(void) const override;
	void Draw(void) override;

private:

	const FrameStats& stats_;
	DebugSetting& settings_;
	float targetFrameMs_;
};