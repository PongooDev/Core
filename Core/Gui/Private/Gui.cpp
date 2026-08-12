#include "pch.h"

#include "Gui/Public/GuiInternal.h"
#include "Gui/Public/CoreLogo.h"
#include "Core/Public/Version.h"

#include "../../../includes/imgui/imgui.h"
#include "../../../includes/imgui/imgui_impl_win32.h"
#include "../../../includes/imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <deque>
#include <vector>
#include <cmath>

#pragma comment(lib, "d3d11.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
	HANDLE GGuiThread = nullptr;
	std::atomic<bool> GGuiRunning = false;
	std::atomic<int> GGuiState = 0;

	SRWLOCK GLogLock = SRWLOCK_INIT;
	std::deque<std::string>* GLogPending = nullptr;
	std::atomic<bool> GLogClosed = false;

	SRWLOCK GTitleLock = SRWLOCK_INIT;
	std::string* GPendingTitle = nullptr;

	SRWLOCK GEndpointLock = SRWLOCK_INIT;
	std::string* GListenEndpoint = nullptr;

	SRWLOCK GPanelLock = SRWLOCK_INIT;
	std::vector<GuiPanel*>* GPanels = nullptr;

	HWND GWindow = nullptr;
	ID3D11Device* GDevice = nullptr;
	ID3D11DeviceContext* GDeviceContext = nullptr;
	IDXGISwapChain* GSwapChain = nullptr;
	ID3D11RenderTargetView* GRenderTarget = nullptr;

	ID3D11ShaderResourceView* GLogoTexture = nullptr;
	HICON GIconLarge = nullptr;
	HICON GIconSmall = nullptr;

	ImFont* GUiFont = nullptr;
	ImFont* GMonoFont = nullptr;
	ImFont* GHeadingFont = nullptr;

	float GUiScale = 1.0f;
	int GActivePanel = 0;
}

namespace Anim
{
	float Approach(float Current, float Target, float Speed)
	{
		const float Delta = ImGui::GetIO().DeltaTime;
		const float Alpha = 1.0f - expf(-Speed * (Delta > 0.1f ? 0.1f : Delta));
		return Current + (Target - Current) * Alpha;
	}

	float EaseOutCubic(float T)
	{
		const float Inv = 1.0f - T;
		return 1.0f - Inv * Inv * Inv;
	}

	ImVec4 Lerp(const ImVec4& A, const ImVec4& B, float T)
	{
		return ImVec4(
			A.x + (B.x - A.x) * T,
			A.y + (B.y - A.y) * T,
			A.z + (B.z - A.z) * T,
			A.w + (B.w - A.w) * T);
	}

	float& StateFor(ImGuiID Id, float Initial)
	{
		static ImGuiStorage Storage;
		float* Value = Storage.GetFloatRef(Id, Initial);
		return *Value;
	}
}

ImFont* GuiDetail::GetMonoFont()
{
	return GMonoFont;
}

ImFont* GuiDetail::GetHeadingFont()
{
	return GHeadingFont;
}

float GuiDetail::GetUiScale()
{
	return GUiScale;
}

bool Gui::IsEnabled()
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	return !Config.bDisableGui && !Config.bIsProd;
}

void Gui::PushLog(const std::string& Line)
{
	if (GLogClosed || !IsEnabled())
		return;

	FGuiScopedLock Lock(GLogLock);

	if (!GLogPending)
		GLogPending = new std::deque<std::string>();

	GLogPending->push_back(Line);
	if (GLogPending->size() > GuiDetail::MaxLogLines)
		GLogPending->pop_front();
}

bool GuiDetail::TakePendingLog(std::deque<std::string>& Out)
{
	FGuiScopedLock Lock(GLogLock);

	if (!GLogPending || GLogPending->empty())
		return false;

	for (std::string& Line : *GLogPending)
		Out.push_back(std::move(Line));

	GLogPending->clear();
	return true;
}

void Gui::SetTitle(const std::string& Title)
{
	if (GLogClosed || !IsEnabled())
		return;

	FGuiScopedLock Lock(GTitleLock);

	if (!GPendingTitle)
		GPendingTitle = new std::string();

	*GPendingTitle = Title;
}

void Gui::SetListenEndpoint(const std::string& Endpoint)
{
	if (!IsEnabled())
		return;

	FGuiScopedLock Lock(GEndpointLock);

	if (!GListenEndpoint)
		GListenEndpoint = new std::string();

	*GListenEndpoint = Endpoint;
}

std::string GuiDetail::GetListenEndpoint()
{
	FGuiScopedLock Lock(GEndpointLock);
	return GListenEndpoint ? *GListenEndpoint : std::string();
}

void Gui::RegisterPanel(GuiPanel* Panel)
{
	if (!Panel)
		return;

	FGuiScopedLock Lock(GPanelLock);

	if (!GPanels)
		GPanels = new std::vector<GuiPanel*>();

	GPanels->push_back(Panel);
}

namespace
{
	LRESULT WINAPI GuiWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;

		switch (msg)
		{
		case WM_SIZE:
			if (GDevice && wParam != SIZE_MINIMIZED)
			{
				if (GRenderTarget) { GRenderTarget->Release(); GRenderTarget = nullptr; }
				GSwapChain->ResizeBuffers(0, LOWORD(lParam), HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);

				ID3D11Texture2D* BackBuffer = nullptr;
				GSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
				if (BackBuffer)
				{
					GDevice->CreateRenderTargetView(BackBuffer, nullptr, &GRenderTarget);
					BackBuffer->Release();
				}
			}
			return 0;

		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_KEYMENU)
				return 0;
			break;

		case WM_CLOSE:

			ExitProcess(0);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	bool CreateWindowAndDevice(WNDCLASSEXW& OutClass)
	{
		if (HMODULE User32 = GetModuleHandleW(L"user32.dll"))
		{
			typedef DPI_AWARENESS_CONTEXT(WINAPI* SetThreadDpiFn)(DPI_AWARENESS_CONTEXT);
			if (auto SetThreadDpi = (SetThreadDpiFn)GetProcAddress(User32, "SetThreadDpiAwarenessContext"))
				SetThreadDpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
		}

		OutClass = { sizeof(OutClass), CS_CLASSDC, GuiWndProc, 0, 0,
			GetModuleHandleW(nullptr), nullptr, LoadCursorW(nullptr, IDC_ARROW),
			nullptr, nullptr, L"CoreGuiWindow", nullptr };
		RegisterClassExW(&OutClass);

		GWindow = CreateWindowW(OutClass.lpszClassName, L"Core | Starting...", WS_OVERLAPPEDWINDOW,
			100, 100, 1000, 640, nullptr, nullptr, OutClass.hInstance, nullptr);

		if (!GWindow)
		{
			Log("Gui: failed to create the window (error " + std::to_string(GetLastError()) + ").");
			return false;
		}

		DXGI_SWAP_CHAIN_DESC SwapDesc = {};
		SwapDesc.BufferCount = 2;
		SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapDesc.OutputWindow = GWindow;
		SwapDesc.SampleDesc.Count = 1;
		SwapDesc.Windowed = TRUE;
		SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		const D3D_FEATURE_LEVEL Levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL Created = {};

		HRESULT Result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, 0,
			Levels, 2, D3D11_SDK_VERSION, &GDevice, &Created, &GDeviceContext);

		if (FAILED(Result))
			Result = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
				Levels, 2, D3D11_SDK_VERSION, &GDevice, &Created, &GDeviceContext);

		if (FAILED(Result) || !GDevice)
		{
			Log("Gui: failed to create a D3D11 device (hr " + std::format("0x{:X}", (unsigned)Result) + ").");
			return false;
		}

		IDXGIDevice* DxgiDevice = nullptr;
		IDXGIAdapter* DxgiAdapter = nullptr;
		IDXGIFactory* DxgiFactory = nullptr;

		Result = GDevice->QueryInterface(IID_PPV_ARGS(&DxgiDevice));
		if (SUCCEEDED(Result))
		{
			Result = DxgiDevice->GetAdapter(&DxgiAdapter);
			if (SUCCEEDED(Result))
				Result = DxgiAdapter->GetParent(IID_PPV_ARGS(&DxgiFactory));
		}

		if (SUCCEEDED(Result) && DxgiFactory)
			Result = DxgiFactory->CreateSwapChain(GDevice, &SwapDesc, &GSwapChain);

		if (DxgiFactory) DxgiFactory->Release();
		if (DxgiAdapter) DxgiAdapter->Release();
		if (DxgiDevice) DxgiDevice->Release();

		if (FAILED(Result) || !GSwapChain)
		{
			Log("Gui: failed to create the swap chain (hr " + std::format("0x{:X}", (unsigned)Result) + ").");
			return false;
		}

		ID3D11Texture2D* BackBuffer = nullptr;
		GSwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));
		if (BackBuffer)
		{
			GDevice->CreateRenderTargetView(BackBuffer, nullptr, &GRenderTarget);
			BackBuffer->Release();
		}

		return true;
	}

	void DestroyWindowAndDevice(WNDCLASSEXW& WindowClass)
	{
		if (GLogoTexture) { GLogoTexture->Release(); GLogoTexture = nullptr; }
		if (GIconLarge) { DestroyIcon(GIconLarge); GIconLarge = nullptr; }
		if (GIconSmall) { DestroyIcon(GIconSmall); GIconSmall = nullptr; }

		if (GRenderTarget) { GRenderTarget->Release(); GRenderTarget = nullptr; }
		if (GSwapChain) { GSwapChain->Release(); GSwapChain = nullptr; }
		if (GDeviceContext) { GDeviceContext->Release(); GDeviceContext = nullptr; }
		if (GDevice) { GDevice->Release(); GDevice = nullptr; }

		if (GWindow)
		{
			DestroyWindow(GWindow);
			GWindow = nullptr;
		}

		UnregisterClassW(WindowClass.lpszClassName, WindowClass.hInstance);
	}

	HICON CreateLogoIcon(int Desired)
	{
		const int W = CoreLogo::Width;
		const int H = CoreLogo::Height;

		const size_t MaskPitch = ((W + 31) / 32) * 4;
		const size_t ColorSize = (size_t)W * H * 4;
		const size_t MaskSize = MaskPitch * H;

		std::vector<unsigned char> Resource(sizeof(BITMAPINFOHEADER) + ColorSize + MaskSize, 0);

		BITMAPINFOHEADER* Header = reinterpret_cast<BITMAPINFOHEADER*>(Resource.data());
		Header->biSize = sizeof(BITMAPINFOHEADER);
		Header->biWidth = W;
		Header->biHeight = H * 2;
		Header->biPlanes = 1;
		Header->biBitCount = 32;
		Header->biCompression = BI_RGB;
		Header->biSizeImage = (DWORD)ColorSize;

		unsigned char* Color = Resource.data() + sizeof(BITMAPINFOHEADER);

		for (int y = 0; y < H; y++)
		{
			const unsigned char* Src = CoreLogo::Pixels + (size_t)y * W * 4;
			unsigned char* Dst = Color + (size_t)(H - 1 - y) * W * 4;

			for (int x = 0; x < W; x++)
			{
				Dst[x * 4 + 0] = Src[x * 4 + 2];
				Dst[x * 4 + 1] = Src[x * 4 + 1];
				Dst[x * 4 + 2] = Src[x * 4 + 0];
				Dst[x * 4 + 3] = Src[x * 4 + 3];
			}
		}

		return CreateIconFromResourceEx(Resource.data(), (DWORD)Resource.size(), TRUE,
			0x00030000, Desired, Desired, LR_DEFAULTCOLOR);
	}

	void ApplyWindowIcon()
	{
		GIconLarge = CreateLogoIcon(GetSystemMetrics(SM_CXICON));
		GIconSmall = CreateLogoIcon(GetSystemMetrics(SM_CXSMICON));

		if (GIconLarge)
			SendMessageW(GWindow, WM_SETICON, ICON_BIG, (LPARAM)GIconLarge);
		if (GIconSmall)
			SendMessageW(GWindow, WM_SETICON, ICON_SMALL, (LPARAM)GIconSmall);
	}

	void CreateLogoTexture()
	{
		D3D11_TEXTURE2D_DESC Desc = {};
		Desc.Width = CoreLogo::Width;
		Desc.Height = CoreLogo::Height;
		Desc.MipLevels = 1;
		Desc.ArraySize = 1;
		Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		Desc.SampleDesc.Count = 1;
		Desc.Usage = D3D11_USAGE_DEFAULT;
		Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA Initial = {};
		Initial.pSysMem = CoreLogo::Pixels;
		Initial.SysMemPitch = CoreLogo::Width * 4;

		ID3D11Texture2D* Texture = nullptr;
		if (FAILED(GDevice->CreateTexture2D(&Desc, &Initial, &Texture)) || !Texture)
			return;

		D3D11_SHADER_RESOURCE_VIEW_DESC View = {};
		View.Format = Desc.Format;
		View.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		View.Texture2D.MipLevels = 1;

		GDevice->CreateShaderResourceView(Texture, &View, &GLogoTexture);
		Texture->Release();
	}

	float GetWindowScale()
	{
		if (HMODULE User32 = GetModuleHandleW(L"user32.dll"))
		{
			typedef UINT(WINAPI* GetDpiForWindowFn)(HWND);
			if (auto GetDpi = (GetDpiForWindowFn)GetProcAddress(User32, "GetDpiForWindow"))
			{
				const UINT Dpi = GetDpi(GWindow);
				if (Dpi >= 96)
					return (float)Dpi / 96.0f;
			}
		}

		return 1.0f;
	}

	ImFont* LoadSystemFont(const char* FileName, float Size)
	{
		char Path[MAX_PATH] = {};
		const UINT Length = GetWindowsDirectoryA(Path, MAX_PATH);
		if (Length == 0 || Length >= MAX_PATH)
			return nullptr;

		strcat_s(Path, "\\Fonts\\");
		strcat_s(Path, FileName);

		if (GetFileAttributesA(Path) == INVALID_FILE_ATTRIBUTES)
			return nullptr;

		return ImGui::GetIO().Fonts->AddFontFromFileTTF(Path, Size);
	}

	void LoadFonts()
	{
		ImGuiIO& IO = ImGui::GetIO();

		GUiFont = LoadSystemFont("segoeui.ttf", 17.0f);
		if (!GUiFont)
			GUiFont = IO.Fonts->AddFontDefaultVector();

		GMonoFont = LoadSystemFont("consola.ttf", 15.0f);
		if (!GMonoFont)
			GMonoFont = GUiFont;

		GHeadingFont = LoadSystemFont("seguisb.ttf", 21.0f);
		if (!GHeadingFont)
			GHeadingFont = LoadSystemFont("segoeuib.ttf", 21.0f);
		if (!GHeadingFont)
			GHeadingFont = GUiFont;

		IO.FontDefault = GUiFont;
	}

	void ApplyStyle()
	{
		ImGuiStyle& Style = ImGui::GetStyle();
		ImVec4* Colors = Style.Colors;

		const ImVec4& Accent = Theme::Accent;
		const ImVec4& AccentSoft = Theme::AccentSoft;
		const ImVec4& AccentBright = Theme::AccentBright;
		const ImVec4& Background = Theme::Background;
		const ImVec4& Surface = Theme::Surface;
		const ImVec4& SurfaceHover = Theme::SurfaceHover;
		const ImVec4& Line = Theme::Line;

		Colors[ImGuiCol_Text] = Theme::Text;
		Colors[ImGuiCol_TextDisabled] = Theme::TextDim;
		Colors[ImGuiCol_TextSelectedBg] = AccentSoft;

		Colors[ImGuiCol_WindowBg] = Background;
		Colors[ImGuiCol_ChildBg] = ImVec4(0.09f, 0.10f, 0.11f, 1.00f);
		Colors[ImGuiCol_PopupBg] = Surface;
		Colors[ImGuiCol_Border] = Line;
		Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		Colors[ImGuiCol_FrameBg] = Surface;
		Colors[ImGuiCol_FrameBgHovered] = SurfaceHover;
		Colors[ImGuiCol_FrameBgActive] = SurfaceHover;

		Colors[ImGuiCol_TitleBg] = Background;
		Colors[ImGuiCol_TitleBgActive] = Surface;
		Colors[ImGuiCol_MenuBarBg] = Surface;

		Colors[ImGuiCol_ScrollbarBg] = Background;
		Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.22f, 0.25f, 0.28f, 1.00f);
		Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.30f, 0.34f, 0.38f, 1.00f);
		Colors[ImGuiCol_ScrollbarGrabActive] = AccentSoft;

		Colors[ImGuiCol_CheckMark] = Accent;
		Colors[ImGuiCol_SliderGrab] = Accent;
		Colors[ImGuiCol_SliderGrabActive] = AccentBright;

		Colors[ImGuiCol_Button] = Surface;
		Colors[ImGuiCol_ButtonHovered] = SurfaceHover;
		Colors[ImGuiCol_ButtonActive] = AccentSoft;

		Colors[ImGuiCol_Header] = Surface;
		Colors[ImGuiCol_HeaderHovered] = SurfaceHover;
		Colors[ImGuiCol_HeaderActive] = SurfaceHover;

		Colors[ImGuiCol_Separator] = Line;
		Colors[ImGuiCol_SeparatorHovered] = AccentSoft;
		Colors[ImGuiCol_SeparatorActive] = Accent;

		Colors[ImGuiCol_Tab] = Background;
		Colors[ImGuiCol_TabHovered] = SurfaceHover;
		Colors[ImGuiCol_TabSelected] = Surface;
		Colors[ImGuiCol_TabSelectedOverline] = Accent;
		Colors[ImGuiCol_TabDimmed] = Background;
		Colors[ImGuiCol_TabDimmedSelected] = Surface;

		Colors[ImGuiCol_TableHeaderBg] = Surface;
		Colors[ImGuiCol_TableBorderStrong] = Line;
		Colors[ImGuiCol_TableBorderLight] = ImVec4(0.15f, 0.17f, 0.19f, 1.00f);
		Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);

		Colors[ImGuiCol_NavCursor] = Accent;

		Style.WindowRounding = 0.0f;
		Style.ChildRounding = 4.0f;
		Style.FrameRounding = 4.0f;
		Style.PopupRounding = 4.0f;
		Style.ScrollbarRounding = 4.0f;
		Style.TabRounding = 4.0f;
		Style.GrabRounding = 4.0f;

		Style.WindowPadding = ImVec2(12.0f, 10.0f);
		Style.FramePadding = ImVec2(8.0f, 4.0f);
		Style.ItemSpacing = ImVec2(8.0f, 6.0f);
		Style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		Style.CellPadding = ImVec2(8.0f, 4.0f);
		Style.ScrollbarSize = 13.0f;

		Style.WindowBorderSize = 0.0f;
		Style.FrameBorderSize = 0.0f;
		Style.TabBarBorderSize = 0.0f;

		GUiScale = GetWindowScale();
		if (GUiScale > 1.0f)
		{
			Style.ScaleAllSizes(GUiScale);
			Style.FontScaleDpi = GUiScale;
		}
	}
}

namespace
{
	bool& CategoryOpenRef(const char* Category)
	{
		static ImGuiStorage Storage;
		return *Storage.GetBoolRef(ImGui::GetID(Category), true);
	}

	bool NavCategory(const char* Label, bool bOpen)
	{
		const float Height = 24.0f * GUiScale;
		const float Indent = 15.0f * GUiScale;

		const ImVec2 Pos = ImGui::GetCursorScreenPos();
		const ImVec2 Size = ImVec2(ImGui::GetContentRegionAvail().x, Height);

		const bool bClicked = ImGui::InvisibleButton(Label, Size);
		const bool bHovered = ImGui::IsItemHovered();

		float& Turn = Anim::StateFor(ImGui::GetItemID(), bOpen ? 1.0f : 0.0f);
		Turn = Anim::Approach(Turn, bOpen ? 1.0f : 0.0f, 16.0f);

		ImDrawList* Draw = ImGui::GetWindowDrawList();

		const float Angle = Turn * 1.5707963f;
		const float Cos = cosf(Angle);
		const float Sin = sinf(Angle);
		const float Arm = 3.4f * GUiScale;
		const ImVec2 Centre = ImVec2(Pos.x + Indent * 0.45f, Pos.y + Height * 0.5f);

		auto Rotate = [&](float X, float Y)
		{
			return ImVec2(Centre.x + X * Cos - Y * Sin, Centre.y + X * Sin + Y * Cos);
		};

		const ImU32 ArrowColor = ImGui::GetColorU32(bHovered ? Theme::Text : Theme::TextDim);
		Draw->AddTriangleFilled(
			Rotate(-Arm * 0.6f, -Arm), Rotate(-Arm * 0.6f, Arm), Rotate(Arm * 0.9f, 0.0f), ArrowColor);

		const ImVec2 TextSize = ImGui::CalcTextSize(Label);
		Draw->AddText(
			ImVec2(Pos.x + Indent, Pos.y + (Height - TextSize.y) * 0.5f),
			ImGui::GetColorU32(bHovered ? Theme::Text : ImVec4(0.40f, 0.44f, 0.49f, 1.0f)),
			Label);

		return bClicked;
	}

	bool NavItem(const char* Label, bool bSelected, int BadgeCount, bool bBadgeAlert, float& OutTop, float& OutBottom)
	{
		const float Height = 36.0f * GUiScale;
		const float Rounding = 7.0f * GUiScale;
		const float TextPad = 15.0f * GUiScale;

		const ImVec2 Pos = ImGui::GetCursorScreenPos();
		const ImVec2 Size = ImVec2(ImGui::GetContentRegionAvail().x, Height);

		const bool bClicked = ImGui::InvisibleButton(Label, Size);
		const bool bHovered = ImGui::IsItemHovered();
		const bool bHeld = ImGui::IsItemActive();

		OutTop = Pos.y;
		OutBottom = Pos.y + Height;

		float& Hover = Anim::StateFor(ImGui::GetItemID(), 0.0f);
		Hover = Anim::Approach(Hover, (bHovered || bSelected) ? 1.0f : 0.0f, 14.0f);

		ImDrawList* Draw = ImGui::GetWindowDrawList();
		const ImVec2 End = ImVec2(Pos.x + Size.x, Pos.y + Size.y);

		if (!bSelected && Hover > 0.001f)
		{
			const float Alpha = (bHeld ? 0.07f : 0.045f) * Hover;
			Draw->AddRectFilled(Pos, End, ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, Alpha)), Rounding);
		}

		const ImVec2 TextSize = ImGui::CalcTextSize(Label);
		const ImVec4 Color = Anim::Lerp(Theme::TextDim, Theme::Text, bSelected ? 1.0f : Hover * 0.75f);
		const float Slide = Hover * 2.0f * GUiScale;

		Draw->AddText(
			ImVec2(Pos.x + TextPad + Slide, Pos.y + (Size.y - TextSize.y) * 0.5f),
			ImGui::GetColorU32(Color), Label);

		if (BadgeCount > 0)
		{
			const std::string Badge = BadgeCount > 99 ? std::string("99+") : std::to_string(BadgeCount);
			const ImVec2 BadgeSize = ImGui::CalcTextSize(Badge.c_str());
			const float PadRight = 10.0f * GUiScale;

			if (bBadgeAlert)
			{
				const float PillPadX = 6.0f * GUiScale;
				const float PillHeight = BadgeSize.y + 2.0f * GUiScale;
				const float PillWidth = BadgeSize.x + PillPadX * 2.0f;

				const ImVec2 PillMin = ImVec2(End.x - PadRight - PillWidth, Pos.y + (Size.y - PillHeight) * 0.5f);
				const ImVec2 PillMax = ImVec2(End.x - PadRight, PillMin.y + PillHeight);

				Draw->AddRectFilled(PillMin, PillMax,
					ImGui::GetColorU32(ImVec4(Theme::Bad.x, Theme::Bad.y, Theme::Bad.z, 0.90f)), PillHeight * 0.5f);
				Draw->AddText(
					ImVec2(PillMin.x + PillPadX, PillMin.y + (PillHeight - BadgeSize.y) * 0.5f),
					ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), Badge.c_str());
			}
			else
			{
				Draw->AddText(
					ImVec2(End.x - PadRight - BadgeSize.x, Pos.y + (Size.y - BadgeSize.y) * 0.5f),
					ImGui::GetColorU32(Theme::TextDim), Badge.c_str());
			}
		}

		return bClicked;
	}

	void RenderSidebar(const std::vector<GuiPanel*>& Panels)
	{
		const float Pad = 14.0f * GUiScale;

		ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Sidebar);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Pad, Pad));

		ImGui::BeginChild("Sidebar", ImVec2(190.0f * GUiScale, 0),
			ImGuiChildFlags_AlwaysUseWindowPadding, ImGuiWindowFlags_NoScrollbar);

		const float LogoSize = 30.0f * GUiScale;

		ImGui::BeginGroup();
		if (GLogoTexture)
		{
			ImGui::Image((ImTextureID)GLogoTexture, ImVec2(LogoSize, LogoSize));
			ImGui::SameLine();
		}

		ImGui::PushFont(GHeadingFont, 0.0f);
		const float WordmarkHeight = ImGui::GetTextLineHeight();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (LogoSize - WordmarkHeight) * 0.5f);
		ImGui::TextUnformatted("Core");
		ImGui::PopFont();
		ImGui::EndGroup();

		ImGui::Dummy(ImVec2(0, 10.0f * GUiScale));

		ImDrawList* NavDraw = ImGui::GetWindowDrawList();
		NavDraw->ChannelsSplit(2);
		NavDraw->ChannelsSetCurrent(1);

		float SelectedTop = 0.0f;
		float SelectedBottom = 0.0f;
		float NavLeft = 0.0f;
		float NavRight = 0.0f;
		bool bHaveSelection = false;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 3.0f * GUiScale));

		const char* LastCategory = nullptr;
		bool bCategoryOpen = true;
		int CategoryIndex = -1;

		static bool bCategoriesInitialised = false;

		for (int i = 0; i < (int)Panels.size(); i++)
		{
			const bool bSelected = (i == GActivePanel);

			const char* Category = Panels[i]->Category();
			if (Category && (!LastCategory || strcmp(Category, LastCategory) != 0))
			{
				++CategoryIndex;

				ImGui::Dummy(ImVec2(0, (i == 0 ? 0.0f : 8.0f) * GUiScale));

				bool& bOpen = CategoryOpenRef(Category);

				if (!bCategoriesInitialised)
					bOpen = (CategoryIndex == 0);

				if (NavCategory(Category, bOpen))
					bOpen = !bOpen;

				bCategoryOpen = bOpen;
				ImGui::Dummy(ImVec2(0, 2.0f * GUiScale));
			}
			else if (!Category)
			{
				bCategoryOpen = true;
			}
			LastCategory = Category;

			if (!bCategoryOpen)
				continue;

			float Top = 0.0f;
			float Bottom = 0.0f;
			const float Left = ImGui::GetCursorScreenPos().x;
			const float Right = Left + ImGui::GetContentRegionAvail().x;

			if (NavItem(Panels[i]->Name(), bSelected, Panels[i]->BadgeCount(), Panels[i]->BadgeIsAlert(), Top, Bottom))
				GActivePanel = i;

			if (bSelected)
			{
				SelectedTop = Top;
				SelectedBottom = Bottom;
				NavLeft = Left;
				NavRight = Right;
				bHaveSelection = true;
			}
		}
		ImGui::PopStyleVar();

		bCategoriesInitialised = true;

		NavDraw->ChannelsSetCurrent(0);

		if (bHaveSelection)
		{
			static float HighlightTop = 0.0f;
			static float HighlightBottom = 0.0f;

			if (HighlightBottom <= 0.0f)
			{
				HighlightTop = SelectedTop;
				HighlightBottom = SelectedBottom;
			}

			HighlightTop = Anim::Approach(HighlightTop, SelectedTop, 18.0f);
			HighlightBottom = Anim::Approach(HighlightBottom, SelectedBottom, 18.0f);

			const float Rounding = 7.0f * GUiScale;
			NavDraw->AddRectFilled(
				ImVec2(NavLeft, HighlightTop), ImVec2(NavRight, HighlightBottom),
				ImGui::GetColorU32(Theme::Surface), Rounding);

			const float BarPad = 9.0f * GUiScale;
			NavDraw->AddRectFilled(
				ImVec2(NavLeft, HighlightTop + BarPad),
				ImVec2(NavLeft + 3.0f * GUiScale, HighlightBottom - BarPad),
				ImGui::GetColorU32(Theme::Accent), 2.0f * GUiScale);
		}

		NavDraw->ChannelsMerge();

		const float FooterHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + Pad;
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - FooterHeight);

		ImDrawList* Draw = ImGui::GetWindowDrawList();
		const ImVec2 Dot = ImGui::GetCursorScreenPos();
		const float DotRadius = 3.5f * GUiScale;
		const bool bReady = Version::Fortnite_Version > 0.0;

		const ImVec4 DotColor = bReady ? Theme::Accent : ImVec4(1.0f, 0.72f, 0.25f, 1.0f);
		const ImVec2 DotCenter = ImVec2(Dot.x + DotRadius, Dot.y + ImGui::GetTextLineHeight() * 0.5f);

		const float Pulse = 0.5f + 0.5f * sinf((float)ImGui::GetTime() * 2.0f);
		Draw->AddCircleFilled(DotCenter, DotRadius + (2.5f + Pulse * 2.0f) * GUiScale,
			ImGui::GetColorU32(ImVec4(DotColor.x, DotColor.y, DotColor.z, 0.16f * (1.0f - Pulse * 0.5f))));

		Draw->AddCircleFilled(DotCenter, DotRadius, ImGui::GetColorU32(DotColor));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + DotRadius * 2.0f + 7.0f * GUiScale);

		if (bReady)
		{
			ImGui::TextUnformatted(std::format("Fortnite {:.2f}", Version::Fortnite_Version).c_str());
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + DotRadius * 2.0f + 7.0f * GUiScale);
			ImGui::TextDisabled("Engine %.2f  CL %d", Version::Engine_Version, Version::Fortnite_CL);
		}
		else
		{
			ImGui::TextUnformatted("Starting...");
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + DotRadius * 2.0f + 7.0f * GUiScale);
			ImGui::TextDisabled("Finding build");
		}

		ImGui::EndChild();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	void RenderFrame()
	{
		const ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(Viewport->WorkPos);
		ImGui::SetNextWindowSize(Viewport->WorkSize);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("##Core", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoSavedSettings);
		ImGui::PopStyleVar();

		std::vector<GuiPanel*> Panels;
		{
			FGuiScopedLock Lock(GPanelLock);
			if (GPanels)
				Panels = *GPanels;
		}

		if (GActivePanel >= (int)Panels.size())
			GActivePanel = 0;

		if (!Panels.empty())
		{
			const int Count = (int)Panels.size();

			if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab))
				GActivePanel = (GActivePanel - 1 + Count) % Count;
			else if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | ImGuiKey_Tab))
				GActivePanel = (GActivePanel + 1) % Count;

			for (int i = 0; i < Count && i < 9; i++)
			{
				if (ImGui::IsKeyChordPressed(ImGuiMod_Ctrl | (ImGuiKey)(ImGuiKey_1 + i)))
					GActivePanel = i;
			}
		}

		RenderSidebar(Panels);

		ImGui::SameLine(0.0f, 0.0f);

		const float Pad = 18.0f * GUiScale;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(Pad, 14.0f * GUiScale));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Theme::Background);
		ImGui::BeginChild("Content", ImVec2(0, 0), ImGuiChildFlags_AlwaysUseWindowPadding);

		if (!Panels.empty())
		{
			GuiPanel* Active = Panels[GActivePanel];

			static int LastPanel = -1;
			static float Enter = 1.0f;

			if (LastPanel != GActivePanel)
			{
				LastPanel = GActivePanel;
				Enter = 0.0f;
			}

			Enter = Anim::Approach(Enter, 1.0f, 16.0f);

			const float Eased = Anim::EaseOutCubic(Enter);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (1.0f - Eased) * 8.0f * GUiScale);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * Eased);

			ImGui::PushFont(GHeadingFont, 0.0f);
			ImGui::TextUnformatted(Active->Name());
			ImGui::PopFont();

			ImGui::Dummy(ImVec2(0, 2.0f * GUiScale));
			ImGui::Separator();
			ImGui::Dummy(ImVec2(0, 4.0f * GUiScale));

			ImGui::PushID(Active->Name());
			Active->Render();
			ImGui::PopID();

			ImGui::PopStyleVar();
		}

		ImGui::EndChild();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void ApplyPendingTitle()
	{
		FGuiScopedLock Lock(GTitleLock);
		if (GPendingTitle && !GPendingTitle->empty())
		{
			SetWindowTextA(GWindow, GPendingTitle->c_str());
			GPendingTitle->clear();
		}
	}

	void TickPanels()
	{
		std::vector<GuiPanel*> Panels;
		{
			FGuiScopedLock Lock(GPanelLock);
			if (GPanels)
				Panels = *GPanels;
		}

		for (GuiPanel* Panel : Panels)
			Panel->Tick();
	}

	DWORD WINAPI GuiThreadProc(LPVOID)
	{
		for (const wchar_t* Module : { L"d3d11.dll", L"d3dcompiler_47.dll", L"imm32.dll" })
		{
			if (!GetModuleHandleW(Module) && !LoadLibraryW(Module))
			{
				Log("Gui: a required graphics module is unavailable - running without the GUI.");
				GGuiRunning = false;
				GGuiState = 2;
				return 1;
			}
		}

		WNDCLASSEXW WindowClass = {};
		if (!CreateWindowAndDevice(WindowClass))
		{
			DestroyWindowAndDevice(WindowClass);
			GGuiRunning = false;
			GGuiState = 2;
			return 1;
		}

		ApplyWindowIcon();

		ShowWindow(GWindow, SW_SHOWNORMAL);
		UpdateWindow(GWindow);
		SetForegroundWindow(GWindow);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().IniFilename = nullptr;
		ImGui::StyleColorsDark();
		LoadFonts();
		ApplyStyle();

		ImGui_ImplWin32_Init(GWindow);
		ImGui_ImplDX11_Init(GDevice, GDeviceContext);

		CreateLogoTexture();

		GGuiState = 1;

		bool bQuit = false;
		bool bOccluded = false;

		while (GGuiRunning && !bQuit)
		{
			MSG Message;
			while (PeekMessageW(&Message, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Message);
				DispatchMessageW(&Message);
				if (Message.message == WM_QUIT)
					bQuit = true;
			}
			if (bQuit)
				break;

			ApplyPendingTitle();
			TickPanels();

			if (bOccluded && GSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
			{
				Sleep(10);
				continue;
			}
			bOccluded = false;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			RenderFrame();

			ImGui::Render();

			if (GRenderTarget)
			{
				const float ClearColor[4] = { 0.06f, 0.06f, 0.07f, 1.0f };
				GDeviceContext->OMSetRenderTargets(1, &GRenderTarget, nullptr);
				GDeviceContext->ClearRenderTargetView(GRenderTarget, ClearColor);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			}

			const HRESULT Present = GSwapChain->Present(1, 0);
			if (Present == DXGI_ERROR_DEVICE_REMOVED || Present == DXGI_ERROR_DEVICE_RESET)
			{
				Log("Gui: D3D11 device lost - closing the GUI window.");
				break;
			}
			bOccluded = Present == DXGI_STATUS_OCCLUDED;
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		DestroyWindowAndDevice(WindowClass);

		GGuiRunning = false;
		return 0;
	}
}

bool Gui::IsRunning()
{
	return GGuiState == 1;
}

void Gui::Start()
{
	if (!IsEnabled() || GGuiRunning)
		return;

	RegisterPanel(GuiDetail::CreateConsolePanel());
	RegisterPanel(GuiDetail::CreateStatusPanel());
	RegisterPanel(GuiDetail::CreatePlayersPanel());
	RegisterPanel(GuiDetail::CreateNetworkPanel());
	RegisterPanel(GuiDetail::CreatePerformancePanel());
	RegisterPanel(GuiDetail::CreateOffsetsPanel());

	GGuiRunning = true;
	GGuiState = 0;
	GGuiThread = CreateThread(nullptr, 0, GuiThreadProc, nullptr, 0, nullptr);

	if (!GGuiThread)
	{
		Log("Gui: failed to create the GUI thread (error " + std::to_string(GetLastError()) + ").");
		GGuiRunning = false;
		GGuiState = 2;
		return;
	}

	for (int Waited = 0; GGuiState == 0 && Waited < 5000; Waited += 10)
		Sleep(10);

	if (GGuiState == 0)
		Log("Gui: window did not come up in time - falling back to the console.");
}

void Gui::Shutdown()
{
	GLogClosed = true;
	GGuiRunning = false;

	GuiDetail::StopConsoleMirror();

	if (GGuiThread)
	{
		CloseHandle(GGuiThread);
		GGuiThread = nullptr;
	}
}
