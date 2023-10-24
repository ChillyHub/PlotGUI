#include "WindowsManager.h"

#ifdef WIN32
	#include "Win32/D3DWindow.h"
#endif

namespace PlotGUI
{
	std::shared_ptr<GuiWindow> WindowsManager::CreateGuiWindow(
		std::wstring name, unsigned int width, unsigned int height)
	{
#ifdef WIN32
		if (m_Windows.find(name) == m_Windows.end())
		{
			m_Windows[name] = std::static_pointer_cast<GuiWindow, D3DWindow>(
				std::make_shared<D3DWindow>(name, width, height));
		}
		return m_Windows[name];
#else
		return std::shared_ptr<GuiWindow>();
#endif

	}

	void WindowsManager::DestroyAllWindows()
	{
		for (auto [name, window] : m_Windows)
		{
			if (window.get())
			{
				window->Destroy();
			}
		}

		m_Windows.clear();
	}

	void WindowsManager::DestroyWindow(const std::wstring& windowName)
	{
		if (m_Windows.find(windowName) != m_Windows.end())
		{
			m_Windows[windowName]->Destroy();
			m_Windows.erase(windowName);
		}
	}
}
