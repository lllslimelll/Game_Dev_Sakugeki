#include <imgui/imgui.h>
#include "DebugSetting.h"
#include "DebugDraw3D.h"
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

	// グリッドと、その高さ（グリッドがオフのときは高さを灰色にする）
	ImGui::Checkbox("グリッド", &setting_.showGrid);
	ImGui::BeginDisabled(!setting_.showGrid);
	ImGui::Indent();
	ImGui::DragFloat("高さ", &setting_.gridHeight, 1.0f);
	ImGui::Unindent();
	ImGui::EndDisabled();

	ImGui::Checkbox("レイ（Linecast）", &setting_.showRays);

	// コライダーと、表示する Layer（コライダーがオフのときは Layer を灰色にする）
	ImGui::Checkbox("コライダー", &setting_.showColliders);
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

		// その Layer の色の四角を、チェックボックスの左に出す
		const unsigned int color = DebugDraw3D::GetLayerColor(i);
		const ImVec4 imColor(
			((color >> 16) & 0xff) / 255.0f,
			((color >> 8) & 0xff) / 255.0f,
			(color & 0xff) / 255.0f,
			1.0f);

		ImGui::PushID(i);
		ImGui::ColorButton("##LayerColor", imColor, ImGuiColorEditFlags_NoTooltip, ImVec2(12.0f, 12.0f));
		ImGui::PopID();
		ImGui::SameLine();

		// ビットを1つだけ切り替えるチェックボックス
		ImGui::CheckboxFlags(layerNames_[i].c_str(), &setting_.colliderLayers, 1u << i);
	}
}