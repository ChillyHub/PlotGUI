#include "Application.h"

#include <iostream>

using namespace PlotGUI;

int main()
{
	try
	{
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
