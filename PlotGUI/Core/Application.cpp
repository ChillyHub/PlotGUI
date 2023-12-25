#include "Application.h"

#include "WindowsManager.h"
#include "ProjectsManager.h"

#include "Component/Panel.h"

namespace PlotGUI
{
	void Application::Init()
	{
		m_MainWindow = WindowsManager::Instance().CreateGuiWindow(L"Plot GUI");

		// Panel Init
		Panel::Init();

		m_MainWindow->InitWindow();
	}

	void Application::Run()
	{
		bool shouldClose = false;
		while (!shouldClose)
		{
			WindowEvent event = m_MainWindow->PollEvent();
			shouldClose = event == WindowEvent::Quit;

			if (shouldClose)
			{
				break;
			}

			if (event == WindowEvent::Pause)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				continue;
			}

			m_MainWindow->Resize();
			m_MainWindow->StartFrame();

			ProjectsManager::Instance().UpdateProjects();

			Panel::UpdateFrame();

			// Example
			Panel::DrawPlotPanel();
			Panel::DrawInspectorPanel();
			Panel::DrawConsolePanel();
			Panel::DrawEditorPanel();

			// Present
			m_MainWindow->Present();
		}
	}

	void Application::Release()
	{
		m_MainWindow->Destroy();
	}
}
