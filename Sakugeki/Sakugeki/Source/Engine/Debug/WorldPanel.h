#pragma once
#include <array>
#include <string>
#include "DebugPanel.h"

struct DebugSetting;

// 3D 空間の表示（軸、グリッド、レイ、コライダー）を切り替えるパネル
class WorldPanel : public DebugPanel
{
public:

	// レイヤーの最大数（CollisionManager::MAX_LAYERS と同じ）
	static constexpr int MAX_LAYERS = 32;

	using LayerNames = std::array<std::string, MAX_LAYERS>;

	// layerNames : Layer の名前（空の Layer はチェックボックスを出さない）
	WorldPanel(DebugSetting& setting, const LayerNames& layerNames);
	~WorldPanel(void) override;

	const char* GetName(void) const override;
	void Draw(void) override;

private:

	DebugSetting& setting_;
	const LayerNames& layerNames_;

	// コライダーを表示する Layer のチェックボックス
	void DrawLayerCheckBoxes(void);
};