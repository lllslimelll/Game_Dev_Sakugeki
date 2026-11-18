#include <filesystem>
#include <DxLib.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx11.h>
#include "DebugGui.h"

// imgui_impl_win32.h の中では、windows.h との衝突を避けるためにコメントアウトされているので、ここで宣言する
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	constexpr const char* FONT_PATH = "C:/Windows/Fonts/meiryo.ttc";
	constexpr float FONT_SIZE = 18.0f;

	// DxLib が受け取ったウィンドウメッセージを、ImGui にも渡す
	LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		{
			// ImGui が処理したメッセージは、DxLib に処理させない
			SetUseHookWinProcReturnValue(TRUE);
			return TRUE;
		}
		return 0;
	}
}

DebugGui::DebugGui(void)
	:
	isInitialized_(false)
{
}

DebugGui::~DebugGui(void)
{
	Release();
}

bool DebugGui::Init(void)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	LoadFont();

	// DxLib が内部で使っているウィンドウと D3D11 のデバイスを、ImGui に渡す
	auto* device = static_cast<ID3D11Device*>(const_cast<void*>(GetUseDirect3D11Device()));
	auto* context = static_cast<ID3D11DeviceContext*>(const_cast<void*>(GetUseDirect3D11DeviceContext()));

	if (!ImGui_ImplWin32_Init(GetMainWindowHandle()))
	{
		ImGui::DestroyContext();
		return false;
	}
	if (!ImGui_ImplDX11_Init(device, context))
	{
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		return false;
	}

	// ウィンドウメッセージを横取りする（ここから ImGui にキー入力などが届く）
	SetHookWinProc(HookWndProc);

	isInitialized_ = true;
	SetActive(false);
	return true;
}

void DebugGui::SetActive(bool isActive)
{
	if (!isInitialized_) { return; }

	ImGuiIO& io = ImGui::GetIO();

	// 開いている間：カーソルは ImGui が自分で描く（ゲームがカーソルを消していても見える）
	// 閉じている間：カーソルの表示は DxLib（ゲーム）に任せ、ImGui は触らない
	io.MouseDrawCursor = isActive;
	if (isActive)
	{
		io.ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
	}
	else
	{
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	}
}

void DebugGui::BeginFrame(void)
{
	if (!isInitialized_) { return; }

	UpdateMouse();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DebugGui::EndFrame(void)
{
	if (!isInitialized_) { return; }

	ImGui::Render();

	// DxLib が溜めている描画を、先に実行しておく
	// （溜まったままだと ScreenFlip のときに描かれて、ImGui の上に重なることがある）
	RenderVertex();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// ImGui が変えた D3D11 の設定を、DxLib の状態に戻す
	RefreshDxLibDirect3DSetting();
}

void DebugGui::Release(void)
{
	if (!isInitialized_) { return; }

	SetHookWinProc(nullptr);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	isInitialized_ = false;
}

bool DebugGui::IsUsingMouse(void) const
{
	if (!isInitialized_) { return false; }
	return ImGui::GetIO().WantCaptureMouse;
}

void DebugGui::LoadFont(void)
{
	ImGuiIO& io = ImGui::GetIO();

	// 標準フォントには日本語がないので、Windows のフォントを読む
	if (std::filesystem::exists(FONT_PATH))
	{
		io.Fonts->AddFontFromFileTTF(FONT_PATH, FONT_SIZE, nullptr, io.Fonts->GetGlyphRangesJapanese());
	}
	else
	{
		io.Fonts->AddFontDefault();
	}
}

void DebugGui::UpdateMouse(void)
{
	// 資料のとおり、マウスはウィンドウメッセージだけでは届かないので、DxLib から直接渡す
	ImGuiIO& io = ImGui::GetIO();

	int x = 0;
	int y = 0;
	GetMousePoint(&x, &y);
	io.AddMousePosEvent(static_cast<float>(x), static_cast<float>(y));

	const int buttons = GetMouseInput();
	io.AddMouseButtonEvent(0, (buttons & MOUSE_INPUT_LEFT) != 0);
	io.AddMouseButtonEvent(1, (buttons & MOUSE_INPUT_RIGHT) != 0);
}