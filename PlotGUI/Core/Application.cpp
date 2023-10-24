#include "Application.h"

#include "WindowsManager.h"

#include "imgui.h"

#include "Component/Panel.h"

namespace PlotGUI
{
	void Application::Init()
	{
		m_MainWindow = WindowsManager::Instance().CreateGuiWindow(L"Plot GUI");

		m_MainWindow->InitWindow();

		// Panel Init
		Panel::Init();
	}

	void Application::Run()
	{
		bool shouldClose = false;
		while (!shouldClose)
		{
			shouldClose = m_MainWindow->PollEvent();

			if (shouldClose)
			{
				break;
			}

			m_MainWindow->Resize();
			m_MainWindow->StartFrame();

			Panel::UpdateFrame();

			// Example
			Panel::DrawPlotPanel();
			Panel::DrawInspectorPanel();
			Panel::DrawConsolePanel();

			// Present
			m_MainWindow->Present();
		}
	}

	void Application::Release()
	{
		m_MainWindow->Destroy();
	}
}
