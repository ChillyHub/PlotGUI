#pragma once

#include <d3d11.h>

#include <memory>
#include <string>

#include "Core/GuiWindow.h"

namespace PlotGUI
{
    class D3DWindow : public GuiWindow
    {
    public:
        D3DWindow(const std::wstring& name, unsigned int width, unsigned int height);
    public:
        virtual void InitWindow() override;
        virtual void Resize() override;
        virtual void StartFrame() override;
        virtual void Present() override;
        virtual void Destroy() override;

        virtual WindowEvent PollEvent() override;

    private:
        // Forward declarations of helper functions
        bool CreateDeviceD3D(HWND hWnd);
        void CleanupDeviceD3D();
        void CreateRenderTarget();
        void CleanupRenderTarget();

        LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public:
        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        bool m_init = false;
        HWND m_hWnd;
        WNDCLASSEXW m_windowClass;
        std::wstring m_windowName;
        ID3D11Device* m_device;
        ID3D11DeviceContext* m_deviceContext;
        IDXGISwapChain* m_swapChain;
        ID3D11RenderTargetView* m_mainRenderTargetView;
    };
}
