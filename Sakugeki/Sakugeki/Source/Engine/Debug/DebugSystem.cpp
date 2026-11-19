#include <cmath>
#include <imgui/imgui.h>
#include <DxLib.h>
#include "DebugConfig.h"
#include "DebugGui.h"
#include "StatsPanel.h"
#include "../Math/Vector3.h"
#include "DebugDraw3D.h"
#include "DebugSystem.h"

namespace
{
	// デバッグ用のキー（F1 は今 GameScene でダメージのテストに使っている）
	constexpr int KEY_VISIBLE = KEY_INPUT_F3;	// デバッグ画面
	constexpr int KEY_PAUSE = KEY_INPUT_F5;		// 一時停止
	constexpr int KEY_STEP = KEY_INPUT_F6;		// コマ送り

	constexpr unsigned int COLOR_PAUSE = 0xffff66;

	constexpr float		   AXIS_LENGTH = 100.0f;		// 1m（1 = 1cm の想定）
	constexpr unsigned int COLOR_AXIS_X = 0xff4040;
	constexpr unsigned int COLOR_AXIS_Y = 0x40ff40;
	constexpr unsigned int COLOR_AXIS_Z = 0x4080ff;

	// グリッド
	constexpr float GRID_SPACING = 100.0f;		// 線の間隔（1m）
	constexpr int GRID_HALF_COUNT = 10;			// 中心から片側に何本引くか（半径 10m）
	constexpr int GRID_MAJOR_EVERY = 5;			// 何本ごとに明るい線にするか（5m）
	constexpr unsigned int COLOR_GRID = 0x505050;
	constexpr unsigned int COLOR_GRID_MAJOR = 0x909090;
}

DebugSystem::DebugSystem(int targetFps)
	:
	setting_(),
	frameStats_(),
	gui_(nullptr),
	panels_(),
	targetFrameMs_(1000.0f / targetFps),
	keyState_(),
	prevKeyState_()
{
}

DebugSystem::~DebugSystem(void)
{
}

void DebugSystem::Init(void)
{
#if USE_DEBUG_TOOLS
	gui_ = std::make_unique<DebugGui>();
	if (!gui_->Init())
	{
		// ImGui が使えなくても、ゲームは動かす（一時停止などのキーは使える）
		OutputDebugString(L"DebugGui の初期化に失敗しました\n");
		gui_.reset();
	}

	AddPanel(std::make_unique<StatsPanel>(frameStats_, setting_, targetFrameMs_));
	AddPanel(std::make_unique<WorldPanel>(setting_, layerNames_));
#endif
}

void DebugSystem::BeginFrame(void)
{
#if USE_DEBUG_TOOLS
	frameStats_.BeginFrame();

	UpdateKeys();
	if (IsKeyTriggered(KEY_VISIBLE))
	{
		SetVisible(!setting_.isVisible);
	}
	if (IsKeyTriggered(KEY_PAUSE))
	{
		setting_.isPaused = !setting_.isPaused;
	}
	if (IsKeyTriggered(KEY_STEP))
	{
		// 動いているときに押したら、止めてから1フレーム進める
		setting_.isPaused = true;
		setting_.isStepRequested = true;
	}

	// ゲームが進むフレームだけ、3D の表示を作り直す（止めている間は残す）
	if (ShouldUpdateGame())
	{
		DebugDraw3D::Clear();
		AddOriginAxes();
		AddGrid();
	}
#endif
}

bool DebugSystem::ShouldUpdateGame(void) const
{
#if USE_DEBUG_TOOLS
	return !setting_.isPaused || setting_.isStepRequested;
#else
	return true;
#endif
}

void DebugSystem::EndUpdate(void)
{
#if USE_DEBUG_TOOLS
	frameStats_.EndUpdate();
	setting_.isStepRequested = false;
#endif
}

void DebugSystem::Draw(void)
{
#if USE_DEBUG_TOOLS
	frameStats_.EndDraw();

	if (gui_ && setting_.isVisible)
	{
		gui_->BeginFrame();
		DrawGui();
		gui_->EndFrame();
	}

	frameStats_.EndDebug();
#endif
}

void DebugSystem::EndFrame(void)
{
#if USE_DEBUG_TOOLS
	frameStats_.EndFlip();
#endif
}

void DebugSystem::Release(void)
{
#if USE_DEBUG_TOOLS
	// パネルが ImGui を使うことはもうないが、念のため先に消す
	panels_.clear();
	if (gui_)
	{
		gui_->Release();
		gui_.reset();
	}
#endif
}

void DebugSystem::AddPanel(std::unique_ptr<DebugPanel> panel)
{
	panels_.push_back({ std::move(panel), true });
}

void DebugSystem::SetLayerName(int layer, const std::string& name)
{
	if (layer < 0 || layer >= WorldPanel::MAX_LAYERS) { return; }
	layerNames_[layer] = name;
}

const DebugSetting& DebugSystem::GetSettings(void) const
{
	return setting_;
}

bool DebugSystem::IsUsingMouse(void) const
{
#if USE_DEBUG_TOOLS
	// 開いている間は、マウスはデバッグ画面のもの
	return gui_ && setting_.isVisible;
#else
	return false;
#endif
}

void DebugSystem::DrawWorld(void)
{
#if USE_DEBUG_TOOLS
	if (!setting_.isVisible) { return; }

	// 描く間だけ奥行きの判定を切る
	if (setting_.drawOnTop)
	{
		SetUseZBuffer3D(FALSE);
	}

	DebugDraw3D::Draw(setting_);

	// 奥行きの判定ありに戻す
	if (setting_.drawOnTop)
	{
		SetUseZBuffer3D(TRUE);
	}
#endif
}

void DebugSystem::UpdateKeys(void)
{
	prevKeyState_ = keyState_;
	GetHitKeyStateAll(keyState_.data());
}

bool DebugSystem::IsKeyTriggered(int key) const
{
	return keyState_[key] != 0 && prevKeyState_[key] == 0;
}

void DebugSystem::SetVisible(bool isVisible)
{
	setting_.isVisible = isVisible;
	if (gui_)
	{
		gui_->SetActive(isVisible);
	}
}

void DebugSystem::DrawGui(void)
{
	// 上のメニューバー：閉じたパネルを開き直す場所
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Panels"))
		{
			for (PanelEntry& entry : panels_)
			{
				ImGui::MenuItem(entry.panel->GetName(), nullptr, &entry.isOpen);
			}
			ImGui::Separator();
			ImGui::MenuItem("ImGui Demo", nullptr, &setting_.showDemoWindow);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// 各パネル（Begin が false でも End は必ず呼ぶ）
	for (PanelEntry& entry : panels_)
	{
		if (!entry.isOpen) { continue; }
		if (ImGui::Begin(entry.panel->GetName(), &entry.isOpen))
		{
			entry.panel->Draw();
		}
		ImGui::End();
	}

	if (setting_.showDemoWindow)
	{
		ImGui::ShowDemoWindow(&setting_.showDemoWindow);
	}
}

void DebugSystem::AddOriginAxes(void) const
{
	const Vector3 start(0.0f, 0.0f, 0.0f);
	DebugDraw3D::Arrow(start, Vector3(AXIS_LENGTH, 0.0f, 0.0f), COLOR_AXIS_X, DebugDraw3D::CATEGORY::AXES);
	DebugDraw3D::Arrow(start, Vector3(0.0f, AXIS_LENGTH, 0.0f), COLOR_AXIS_Y, DebugDraw3D::CATEGORY::AXES);
	DebugDraw3D::Arrow(start, Vector3(0.0f, 0.0f, AXIS_LENGTH), COLOR_AXIS_Z, DebugDraw3D::CATEGORY::AXES);
}

void DebugSystem::AddGrid(void) const
{
	// 中心：注視点を 1m 単位にそろえる（線が地面に固定されて見えるように）
	const Vector3 target = Vector3::GetVector3(GetCameraTarget());
	const float centerX = std::floor(target.x / GRID_SPACING) * GRID_SPACING;
	const float centerZ = std::floor(target.z / GRID_SPACING) * GRID_SPACING;
	const float y = setting_.gridHeight;
	const float half = GRID_SPACING * GRID_HALF_COUNT;

	for (int i = -GRID_HALF_COUNT; i <= GRID_HALF_COUNT; i++)
	{
		const float offset = GRID_SPACING * i;

		// 原点からの位置が 5m の倍数なら明るい線（中心からではなく、世界の座標で数える）
		const int lineX = static_cast<int>(std::lround((centerX + offset) / GRID_SPACING));
		const int lineZ = static_cast<int>(std::lround((centerZ + offset) / GRID_SPACING));
		const unsigned int colorX = (lineX % GRID_MAJOR_EVERY == 0) ? COLOR_GRID_MAJOR : COLOR_GRID;
		const unsigned int colorZ = (lineZ % GRID_MAJOR_EVERY == 0) ? COLOR_GRID_MAJOR : COLOR_GRID;

		// Z 方向に伸びる線（X が一定）
		DebugDraw3D::Line(
			Vector3(centerX + offset, y, centerZ - half),
			Vector3(centerX + offset, y, centerZ + half),
			colorX, DebugDraw3D::CATEGORY::GRID);

		// X 方向に伸びる線（Z が一定）
		DebugDraw3D::Line(
			Vector3(centerX - half, y, centerZ + offset),
			Vector3(centerX + half, y, centerZ + offset),
			colorZ, DebugDraw3D::CATEGORY::GRID);
	}
}
