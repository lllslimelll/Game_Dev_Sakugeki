#include <cstdio>
#include <imgui/imgui.h>
#include "DebugSetting.h"
#include "FrameStats.h"
#include "StatsPanel.h"

namespace
{
	constexpr float GRAPH_HEIGHT = 60.0f;
}

StatsPanel::StatsPanel(const FrameStats& stats, DebugSetting& settings, float targetFrameMs)
	:
	stats_(stats),
	setting_(settings),
	targetFrameMs_(targetFrameMs)
{
}

StatsPanel::~StatsPanel(void)
{
}

const char* StatsPanel::GetName(void) const
{
	return "Stats";
}

void StatsPanel::Draw(void)
{
	// 数値
	ImGui::Text("FPS %.1f", stats_.GetFps());
	ImGui::Text("Frame  %.2f ms (max %.2f)", stats_.GetAverageFrameMs(), stats_.GetMaxFrameMs());
	ImGui::Text("Update %.2f  Draw %.2f  Debug %.2f  Flip %.2f",
		stats_.GetUpdateMs(), stats_.GetDrawMs(), stats_.GetDebugMs(), stats_.GetFlipMs());

	// フレーム時間のグラフ（上端は目標の2倍）
	const auto& history = stats_.GetHistory();
	char overlay[32];
	std::snprintf(overlay, sizeof(overlay), "目標 %.1f ms", targetFrameMs_);
	ImGui::PlotHistogram("##FrameTime", history.data(), static_cast<int>(history.size()),
		stats_.GetHistoryHead(), overlay, 0.0f, targetFrameMs_ * 2.0f, ImVec2(0.0f, GRAPH_HEIGHT));

	ImGui::Separator();

	// 一時停止とコマ送り
	ImGui::Checkbox("一時停止 (F5)", &setting_.isPaused);
	ImGui::SameLine();
	if (ImGui::Button("コマ送り (F6)"))
	{
		setting_.isPaused = true;
		setting_.isStepRequested = true;
	}
}