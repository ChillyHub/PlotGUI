#pragma once

#include "imgui.h"

#include <string>

namespace PlotGUI
{
	class Panel
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

	private:
		static void DrawDockingSpace();

	private:
		static int s_CurrentPage;
		static bool s_ShowEditor;
		static std::string s_CurrentProject;
	};
}
