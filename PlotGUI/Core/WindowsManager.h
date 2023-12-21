#pragma once

#include "Utils/Singleton.h"
#include "GuiWindow.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace PlotGUI
{
	class WindowsManager : public Singleton<WindowsManager>
	{
	public:
		std::shared_ptr<GuiWindow> CreateGuiWindow(
			std::wstring name, unsigned int width = 1200, unsigned int height = 800);

		std::shared_ptr<GuiWindow> GetWindow(const std::wstring& windowName);

		void DestroyAllWindows();
		void DestroyWindow(const std::wstring& windowName);

	private:
		std::unordered_map<std::wstring, std::shared_ptr<GuiWindow>> m_Windows;
	};
}
