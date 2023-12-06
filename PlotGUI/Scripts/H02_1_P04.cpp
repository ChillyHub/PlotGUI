#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;

RCLASS H02_1_P04 : public Script
{
public:
	int matrixDim = 4;
	float alpha = 0.5;

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
		ImGui::Spacing();

		ImGui::TextWrapped(
			"Work 01: practice04 \n\
		Write a program that implements an approximate solution to the system of linear algebraic equations of relaxation methods (9). \
		Investigate, for different n and alpha, the dependence of the convergence rate of this iterative method on the iteration parameter \
		tau_{k+1} = tau when numerically solving the system of equations\n\
		Investigate the dependence of the number of iterations on n and the parameter alpha for 0 <= alpha <= 1.\n\
		Construct graphs of the convergence of the number of iterations from n and from alpha.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H02_1_P04");

		ImGui::Spacing();
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;
		desc.lineFlags |= ImPlotLineFlags_SkipNaN;

		// Plot::Plot1("Calculate Time of different dimensions", "dim", "time(ms)", dims.data(), times.data(), times.size(), "Time", desc);
	}

private:
	

private:
	

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H02_1_P04", createFunc, castFunc, "Project")
			.RegisterField("matrixDim", &H02_1_P04::matrixDim, "int", "ReadOnly")
			.RegisterField("alpha", &H02_1_P04::alpha, "float", "Range(0.0,1.0)");
	}
};  RCLASS_END(H02_1_P04)
