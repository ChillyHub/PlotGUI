#include "Application.h"
#include "ProjectsManager.h"

#include <iostream>

using namespace PlotGUI;

int main(int argc, char** argv)
{
	try
	{
		const char* start = "StartScript";
		if (argc > 1)
		{
			start = argv[1];
		}

		ProjectsManager::Instance().ActiveProject(start);

		Application::Instance().Init();
		Application::Instance().Run();
		Application::Instance().Release();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}
