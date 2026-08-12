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

	void ApplyStyle()
	{
		ImGuiStyle& Style = ImGui::GetStyle();
		Style.WindowRounding = 0.0f;
		Style.FrameRounding = 3.0f;
		Style.ScrollbarRounding = 3.0f;
		Style.TabRounding = 3.0f;
		Style.FramePadding = ImVec2(6.0f, 3.0f);
		Style.ItemSpacing = ImVec2(8.0f, 5.0f);
		Style.WindowBorderSize = 0.0f;
	}
}

namespace
{
	void RenderHeader()
	{
		constexpr float LogoSize = 34.0f;

		if (GLogoTexture)
		{
			ImGui::Image((ImTextureID)GLogoTexture, ImVec2(LogoSize, LogoSize));
			ImGui::SameLine();
		}

		const float TextHeight = ImGui::GetTextLineHeight();
		const float Top = ImGui::GetCursorPosY();

		ImGui::BeginGroup();
		ImGui::SetCursorPosY(Top + (LogoSize * 0.5f) - TextHeight - 1.0f);
		ImGui::TextUnformatted("Core");

		ImGui::SetCursorPosY(Top + (LogoSize * 0.5f) + 1.0f);
		if (Version::Fortnite_Version > 0.0)
			ImGui::TextDisabled("Fortnite %.2f  |  Engine %.2f", Version::Fortnite_Version, Version::Engine_Version);
		else
			ImGui::TextDisabled("Starting...");
		ImGui::EndGroup();

		ImGui::SetCursorPosY(Top + LogoSize + 4.0f);
		ImGui::Separator();
	}

	void RenderFrame()
	{
		const ImGuiViewport* Viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(Viewport->WorkPos);
		ImGui::SetNextWindowSize(Viewport->WorkSize);

		ImGui::Begin("##Core", nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoSavedSettings);

		RenderHeader();

		if (ImGui::BeginTabBar("CoreTabs"))
		{
			std::vector<GuiPanel*> Panels;
			{
				FGuiScopedLock Lock(GPanelLock);
				if (GPanels)
					Panels = *GPanels;
			}

			for (GuiPanel* Panel : Panels)
			{
				if (ImGui::BeginTabItem(Panel->Name()))
				{
					Panel->Render();
					ImGui::EndTabItem();
				}
			}

			ImGui::EndTabBar();
		}

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
