#include "Panel.h"
#include "Console.h"

#include "Core/ProjectsManager.h"

#include <iostream>
#include <filesystem>

#include "Component/Plot.h"
#include "Reflection/Include/Reflect.h"

#include "misc/fonts/IconsFontAwesome6.h"
#include "misc/fonts/IconsFontAwesome6Brands.h"

class StartScript;

namespace PlotGUI
{
    int Panel::s_CurrentPage = 1;
    bool Panel::s_ShowEditor = false;
    std::string Panel::s_CurrentProject = "StartScript";

	void Panel::Init()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
        ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;
        //io.ConfigViewportsNoDefaultParent = true;
        //io.ConfigDockingAlwaysTabBar = true;
        //io.ConfigDockingTransparentPayload = true;
        //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
        io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

		// Setup Dear ImGui style
        //ImGui::StyleColorsLight();
        //ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
		SetDarkStyle();
        //SetLightStyle();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

		// Load Fonts
		float fontsize = 20.0f;
		io.Fonts->AddFontFromFileTTF(PROJET_DIR "ThirdPartys/Imgui/misc/fonts/msyh.ttc",
			fontsize, NULL, io.Fonts->GetGlyphRangesChineseFull());
		//io.Fonts->AddFontFromFileTTF(PROJET_DIR "ThirdPartys/Imgui/misc/fonts/msyhbd.ttc",
		//	fontsize, NULL, io.Fonts->GetGlyphRangesChineseFull());

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphMinAdvanceX = fontsize;
        io.Fonts->AddFontFromFileTTF(PROJET_DIR "ThirdPartys/Imgui/misc/fonts/" FONT_ICON_FILE_NAME_FAS, fontsize, &icons_config, S_A6_Icon_Range);

        // Console
        io.Fonts->AddFontFromFileTTF(PROJET_DIR "ThirdPartys/Imgui/misc/fonts/consola.ttf",
            fontsize * 0.9f, NULL);
	}

	void Panel::Destory()
	{
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
	}

	void Panel::SetStyle(const ImGuiStyle& style)
	{
		ImGuiStyle& destStyle = ImGui::GetStyle();
		destStyle = style;
	}

	void Panel::UpdateFrame()
	{
        DrawDockingSpace();
	}

	void Panel::DrawPanel()
	{
	}

	void Panel::DrawPlotPanel()
	{
        if (ImGui::Begin(" " ICON_FA_DIAGRAM_PROJECT "  Plot "))
        {
            Plot::Reset();

            if (ImGui::Button(" " ICON_FA_LEFT_LONG " "))
            {
                Plot::CurrPages--;

                if (Plot::CurrPages < 1 && Plot::TotalPages > 0)
                {
                    Plot::CurrPages = 1;
                    Console::LogInfo("The first plot");
                }
            }
            ImGui::SameLine();

            if (ImGui::Button(" " ICON_FA_RIGHT_LONG " "))
            {
                Plot::CurrPages++;

                if (Plot::CurrPages > Plot::TotalPages && Plot::TotalPages > 0)
                {
                    Plot::CurrPages = Plot::TotalPages;
                    Console::LogInfo("The last plot");
                }
            }
            ImGui::SameLine();

            ImGui::Text(" Page: %d", Plot::CurrPages);

            ProjectsManager::Instance().OnPlotProjects();
        }

        ImGui::End();
	}

	void Panel::DrawInspectorPanel()
	{
        if (ImGui::Begin(" " ICON_FA_INFO "  Inspector "))
        {
	        try
	        {
		        for (const auto& [name, state] : ProjectsManager::Instance().GetProjectStates())
		        {
			        if (state.isActive)
			        {
                        s_CurrentProject = name;

                        auto proj = ProjectsManager::Instance().GetProject(s_CurrentProject);

                        proj->OnInspector();

                        break;
			        }
                    s_CurrentProject = std::string();
		        }
	        }
	        catch (const std::exception& e)
	        {
                ProjectsManager::Instance().DisableProject(s_CurrentProject);
                Console::LogError(e.what());
	        }
        }

        ImGui::End();
	}

	void Panel::DrawConsolePanel()
	{
        if (ImGui::Begin(" " ICON_FA_TERMINAL "  Console "))
        {
            bool open = true;

            Console::Instance().Draw("Title", &open);
        }

        ImGui::End();
	}

	void Panel::DrawEditorPanel()
	{
		if (s_ShowEditor)
		{
            ImGui::ShowStyleEditor();
		}
	}

	void Panel::SetLightStyle()
	{
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 13.0f;
        style.ChildRounding = 13.0f;
        style.FrameRounding = 13.0f;
        style.GrabRounding = 13.0f;
        style.PopupRounding = 13.0f;
        style.ScrollbarRounding = 13.0f;
        style.TabRounding = 5.0f;

        auto& colors = style.Colors;

        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

        colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);

        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

        colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);

        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);

        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);

        colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.59f, 0.26f, 1.00f);

        colors[ImGuiCol_SliderGrab] = ImVec4(0.98f, 0.59f, 0.26f, 0.78f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.80f, 0.54f, 0.46f, 0.60f);

        colors[ImGuiCol_Button] = ImVec4(0.98f, 0.59f, 0.26f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.59f, 0.26f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.53f, 0.06f, 1.00f);

        colors[ImGuiCol_Header] = ImVec4(0.98f, 0.59f, 0.26f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.59f, 0.26f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.59f, 0.26f, 1.00f);

        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.14f, 0.44f, 0.80f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.14f, 0.44f, 0.80f, 1.00f);

        colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.59f, 0.26f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.59f, 0.26f, 0.95f);

        colors[ImGuiCol_Tab] = ImVec4(0.90f, 0.50f, 0.20f, 0.30f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImVec4(0.92f, 0.60f, 0.33f, 0.75f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.95f, 0.88f, 0.66f, 0.90f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.92f, 0.60f, 0.33f, 0.75f);

        colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);

        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);

        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.59f, 0.26f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.98f, 0.59f, 0.26f, 0.95f);
        colors[ImGuiCol_NavHighlight] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
	}

	void Panel::SetDarkStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 13.0f;
		style.ChildRounding = 13.0f;
		style.FrameRounding = 13.0f;
		style.GrabRounding = 13.0f;
		style.PopupRounding = 13.0f;
		style.ScrollbarRounding = 13.0f;
		style.TabRounding = 5.0f;

		auto& colors = style.Colors;
        
        // Text
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Window
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.94f);

        // Borders
        colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Menus
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

        // Scrollbars
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);

        // Check Mark
        colors[ImGuiCol_CheckMark] = ImVec4(0.56f, 0.59f, 0.58f, 1.00f);

        // Sliders
        colors[ImGuiCol_SliderGrab] = ImVec4(0.54f, 0.52f, 0.58f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.59f, 0.58f, 1.00f);

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Separator
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.40f, 0.40f, 0.45f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);

        // Resize Grip
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.56f, 0.59f, 0.58f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.59f, 0.58f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.56f, 0.59f, 0.58f, 0.95f);

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Docking
        colors[ImGuiCol_DockingPreview] = ImVec4{ 0.15f, 0.1505f, 0.151f, 0.7f };
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

        // Plot
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

        // Table
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

        // Others
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.56f, 0.59f, 0.58f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.56f, 0.59f, 0.58f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	void Panel::DrawDockingSpace()
	{
        //_______________________________________________________________________________________________
        // SHOW DOCKING SPACE ///////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
        static bool dockspaceOpen = true;
        static bool opt_fullscreen_persistant = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockingSpace", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 420.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Projects"))
            {
	            for (auto& [name, state] : ProjectsManager::Instance().GetProjectStates())
	            {
		            if (ImGui::MenuItem(name.c_str()))
		            {
                        ProjectsManager::Instance().DisableProject(s_CurrentProject);
                        ProjectsManager::Instance().ActiveProject(name);

                        s_CurrentProject = name;
		            }
	            }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Dark Style"))
                {
                    SetDarkStyle();
                }
                if (ImGui::MenuItem("Light Style"))
                {
                    SetLightStyle();
                }
                if (ImGui::MenuItem("Custom Style"))
                {
                    s_ShowEditor = !s_ShowEditor;
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("New Window"))
                {

                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About PlotGUI"))
                {

                }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        //_______________________________________________________________________________________________
        // SHOW DOCKING SPACE END ///////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////////////////////
	}
}
