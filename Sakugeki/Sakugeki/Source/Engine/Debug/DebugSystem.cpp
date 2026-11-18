#include <imgui/imgui.h>
#include <DxLib.h>
#include "DebugConfig.h"
#include "DebugGui.h"
#include "StatsPanel.h"
#include "DebugSystem.h"

namespace
{
	// デバッグ用のキー（F1 は今 GameScene でダメージのテストに使っている）
	constexpr int KEY_VISIBLE = KEY_INPUT_F3;	// デバッグ画面
	constexpr int KEY_PAUSE = KEY_INPUT_F5;		// 一時停止
	constexpr int KEY_STEP = KEY_INPUT_F6;		// コマ送り

	constexpr unsigned int COLOR_PAUSE = 0xffff66;
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