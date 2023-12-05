#include "Include/PlotGUI.h"

using namespace PlotGUI;

RCLASS TecondScript : public Script
{
public:
	void Start() override
	{
		
	}

	void Update() override
	{
		Console::LogInfo("Second");
	}

	void OnChange() override
	{
		
	}

	void OnPlot() override
	{
		
	}

	void OnInspector() override
	{
		
	}

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("SecondScript", createFunc, castFunc, "Project");
	}
};  RCLASS_END(TecondScript)
