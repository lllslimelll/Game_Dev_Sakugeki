#include <imgui/imgui.h>
#include "DebugSetting.h"
#include "WorldPanel.h"

WorldPanel::WorldPanel(DebugSetting& setting, const LayerNames& layerNames)
	:
	setting_(setting),
	layerNames_(layerNames)
{
}

WorldPanel::~WorldPanel(void)
{
}

const char* WorldPanel::GetName(void) const
{
	return "World";
}

void WorldPanel::Draw(void)
{
	ImGui::SeparatorText("表示");
	ImGui::Checkbox("原点の軸", &setting_.showOriginAxes);
	ImGui::Checkbox("グリッド", &setting_.showGrid);
	ImGui::Checkbox("レイ（Linecast）", &setting_.showRays);
	ImGui::Checkbox("コライダー", &setting_.showColliders);

	// コライダーがオフのときは、Layer の選択を灰色にして触れなくする
	ImGui::BeginDisabled(!setting_.showColliders);
	ImGui::Indent();
	DrawLayerCheckBoxes();
	ImGui::Unindent();
	ImGui::EndDisabled();

	ImGui::SeparatorText("描き方");
	ImGui::Checkbox("前面に表示", &setting_.drawOnTop);
}

void WorldPanel::DrawLayerCheckBoxes(void)
{
	if (ImGui::SmallButton("すべて"))
	{
		setting_.colliderLayers = 0xFFFFFFFFu;
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("なし"))
	{
		setting_.colliderLayers = 0u;
	}

	for (int i = 0; i < MAX_LAYERS; i++)
	{
		// 名前が登録されていない Layer は使われていない
		if (layerNames_[i].empty()) { continue; }

		// ビットを1つだけ切り替えるチェックボックス
		ImGui::CheckboxFlags(layerNames_[i].c_str(), &setting_.colliderLayers, 1u << i);
	}
}