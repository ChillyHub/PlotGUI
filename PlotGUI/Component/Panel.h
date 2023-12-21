#pragma once

#include "imgui.h"
#include "Utils/Singleton.h"

#include <string>
#include <thread>
#include <mutex>

namespace PlotGUI
{
	class Panel : public Singleton<Panel>
	{
	public:
		static void Init();
		static void Destory();

		static void SetStyle(const ImGuiStyle& style);

		static void UpdateFrame();

		static void DrawPanel();
		static void DrawPlotPanel();
		static void DrawInspectorPanel();
		static void DrawConsolePanel();
		static void DrawEditorPanel();

		static void SetLightStyle();
		static void SetDarkStyle();

	public:
		Panel() = default;

	private:
		static void DrawDockingSpace();
		static void UpdateFPSThread();

	private:
		int m_CurrentPage = 1;
		bool m_ShowEditor = false;
		std::string m_CurrentProject = "StartScript";
		int m_FrameCount = 0;
		int m_Fps = 0;

		std::thread m_FpsThread;
	};
}
