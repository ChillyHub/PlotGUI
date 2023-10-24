#include "D3DWindow.h"
#include "Utils/Utils.h"

#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "Component/Panel.h"

#include <stdexcept>

#include <iostream>

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace PlotGUI
{
    unsigned int GuiWindow::s_resizeWidth = 0;
    unsigned int GuiWindow::s_resizeHeight = 0;

	D3DWindow::D3DWindow(const std::wstring& name, unsigned int width, unsigned int height)
	{
        m_windowName = name;

        s_resizeWidth = width;
        s_resizeHeight = height;
	}

	void D3DWindow::InitWindow()
	{
        // Create application window (Hi-DPI)
        ImGui_ImplWin32_EnableDpiAwareness();

        WNDCLASSEXW wc = 
        {
        	sizeof(wc),
        	CS_CLASSDC,
        	WndProc,
        	0L,
        	0L,
        	GetModuleHandle(nullptr),
        	nullptr,
        	nullptr,
        	nullptr,
        	nullptr,
        	L"Plot GUI",
        	nullptr
        };
        ::RegisterClassExW(&wc);

        RECT rect;
        rect.top = 0;
        rect.left = 0;
        rect.right = static_cast<LONG>(s_resizeWidth);
        rect.bottom = static_cast<LONG>(s_resizeHeight);
        ::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        HWND hwnd = ::CreateWindowW(
            wc.lpszClassName, 
            m_windowName.c_str(), 
            WS_OVERLAPPEDWINDOW, 
            CW_USEDEFAULT, CW_USEDEFAULT,
            rect.right - rect.left, rect.bottom - rect.top,
            nullptr, nullptr, 
            wc.hInstance, nullptr);

        // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

            ThrowRuntimeError("Create Device D3D Failed");
        }

        // Init panel
        Panel::Init();

        // Setup Platform/Renderer backends
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(m_device, m_deviceContext);

        m_hWnd = hwnd;
        m_windowClass = wc;
	}

	bool D3DWindow::PollEvent()
	{
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                return true;
        }

        return false;
	}

	void D3DWindow::Resize()
	{
        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (s_resizeWidth != 0 && s_resizeHeight != 0)
        {
            CleanupRenderTarget();
            m_swapChain->ResizeBuffers(0, s_resizeWidth, s_resizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            s_resizeWidth = s_resizeHeight = 0;
            CreateRenderTarget();
        }
	}

	void D3DWindow::StartFrame()
	{
        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (!m_init)
        {
            // Show the window
            ::ShowWindow(m_hWnd, SW_SHOWDEFAULT);
            ::UpdateWindow(m_hWnd);
            m_init = true;
        }
	}

	void D3DWindow::Present()
	{
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        m_deviceContext->OMSetRenderTargets(1, &m_mainRenderTargetView, nullptr);
        m_deviceContext->ClearRenderTargetView(m_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        m_swapChain->Present(1, 0);
	}

	void D3DWindow::Destroy()
	{
        // Cleanup
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();

        Panel::Destory();

        CleanupDeviceD3D();
        ::DestroyWindow(m_hWnd);
        ::UnregisterClassW(m_windowClass.lpszClassName, m_windowClass.hInstance);
	}

	bool D3DWindow::CreateDeviceD3D(HWND hWnd)
	{
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = 
        {
        	D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0
        };

        HRESULT res = D3D11CreateDeviceAndSwapChain(
            nullptr, 
            D3D_DRIVER_TYPE_HARDWARE, 
            nullptr, 
            createDeviceFlags, 
            featureLevelArray, 
            2, 
            D3D11_SDK_VERSION, 
            &sd,
            &m_swapChain,
            &m_device, 
            &featureLevel, 
            &m_deviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(
                nullptr, 
                D3D_DRIVER_TYPE_WARP, 
                nullptr, 
                createDeviceFlags, 
                featureLevelArray, 
                2, 
                D3D11_SDK_VERSION, 
                &sd,
                &m_swapChain,
                &m_device,
                &featureLevel,
                &m_deviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
	}

	void D3DWindow::CleanupDeviceD3D()
	{
        CleanupRenderTarget();
        if (m_swapChain)
        {
	        m_swapChain->Release();
        	m_swapChain = nullptr;
        }
        if (m_deviceContext)
        {
            m_deviceContext->Release();
            m_deviceContext = nullptr;
        }
        if (m_device)
        {
	        m_device->Release();
        	m_device = nullptr;
        }
	}

	void D3DWindow::CreateRenderTarget()
	{
        ID3D11Texture2D* pBackBuffer;
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        m_device->CreateRenderTargetView(pBackBuffer, nullptr, &m_mainRenderTargetView);
        pBackBuffer->Release();
	}

	void D3DWindow::CleanupRenderTarget()
	{
        if (m_mainRenderTargetView)
        {
            m_mainRenderTargetView->Release();
            m_mainRenderTargetView = nullptr;
        }
	}

#ifndef WM_DPICHANGED
	#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

	LRESULT D3DWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        switch (msg)
        {
        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
                return 0;
            s_resizeWidth = LOWORD(lParam); // Queue resize
            s_resizeHeight = HIWORD(lParam);
            return 0;
        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        case WM_DPICHANGED:
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
            {
                // const int dpi = HIWORD(wParam);
                // printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
                const RECT* suggested_rect = (RECT*)lParam;
                ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
            }
            break;
        }
        return ::DefWindowProcW(hWnd, msg, wParam, lParam);
	}
}
