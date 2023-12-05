#include "Include/PlotGUI.h"

using namespace PlotGUI;

RCLASS StartScript : public Script
{
public:
	int intNum = 0;
	float floatNum = 1;
	Math::Vec4 vec4Num = { 0, 1, 2, 3 };
	std::string str = "Hello";

public:
	void Start() override
	{
		
	}

	void Update() override
	{
		
	}

	void OnChange() override
	{
		
	}

	void OnInspector() override
	{
		Inspector::ShowRegisteredFields(this, "StartScript");
	}

	void OnPlot() override
	{
		PlotDescriptor desc;

		Math::Vec4 x{0, 1, 2, 3};
		Math::Vec4 y{0, 1, 4, 9};

		y *= floatNum;

		Plot::Plot1("Start", "", "", x.data(), y.data(), 4, "X", desc);
	}

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("StartScript", createFunc, castFunc, "Project")
			.RegisterField("intNum", &StartScript::intNum, "int")
			.RegisterField("floatNum", &StartScript::floatNum, "float")
			.RegisterField("vec4Num", &StartScript::vec4Num, "vec4")
			.RegisterField("str", &StartScript::str, "string");
	}
};  RCLASS_END(StartScript)
