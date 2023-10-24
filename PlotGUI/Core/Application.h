#pragma once

#include "Utils/Singleton.h"

#include "GuiWindow.h"

#include <memory>

namespace PlotGUI
{
	class Application : public Singleton<Application>
	{
	public:
		void Init();
		void Run();
		void Release();

	private:
		std::shared_ptr<GuiWindow> m_MainWindow;
	};
}
