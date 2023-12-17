#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H05_1_P02 : public Script
{
public: // Inspector Field
	std::string fExpression = " f(x) = e^(-x) * x";
	std::string gExpression = " g(x) = x - 2";

	double a = 4.0;
	double b = 10.0;

private: // Custom Struct

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		if (mSolveValue)
		{
			double minX = MinimizationGoldenSection(F, G, a, b);
			double minY = F(minX);

			Console::Log("======================================================");
			Console::Log("Function %s", fExpression.c_str());
			Console::Log("And      %s", gExpression.c_str());
			Console::Log("In range [%f, %f]: ", a, b);
			Console::Log("When x = %f, min value = %f", minX, minY);
			Console::Log("======================================================");
			Console::Log("\n");
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 05: practice02");
		ImGui::TextWrapped("Write a program to find the minimum of a function of one variable f(x) on the interval [a, b] \
under the constraints g(x) >= 0 by the penalty method using the golden section method to solve the unconstrained minimization problem. \
Using it, find the minimum of the function e^(-x) * x for x >= 2. Draw a graph of the convergence of the method.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H05_1_P02");

		ImGui::Spacing();

		mSolveValue = ImGui::Button("Solve Minimization", { -1, 0 });
		mPlotLoss |= mSolveValue;
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::Plot2G<double>("Function", "", "", 400, F, fExpression, G, gExpression, desc1);

		if (mPlotLoss)
		{
			PlotDescriptor desc2;
			desc2.axisFlags |= ImPlotAxisFlags_AutoFit;
			
			Plot::Plot1("Golden Section Minimization Loss", "Iteration Count", "Loss Value",
				mIterations.data(), mLossValues.data(), mIterations.size(), "Golden Section Method", desc2);
		}
	}

private: // Class Function
	double MinimizationGoldenSection(const std::function<double(double)>& f, const std::function<double(double)>& g, double a, double b)
	{
		const double rate = (Sqrt(5.0) - 1.0) / 2.0;

		double x1 = a + (b - a) * rate;
		double y1 = f(x1) + Pow(Min(0.0, g(x1)), 2.0) / FLT_EPSILON;

		double x2 = a + (b - a) * (1.0 - rate);
		double y2 = f(x2) + Pow(Min(0.0, g(x2)), 2.0) / FLT_EPSILON;

		int i = 0;
		mIterations.clear();
		mLossValues.clear();

		while (true)
		{
			mIterations.push_back(static_cast<double>(++i));
			mLossValues.push_back(x1 - x2);

			if (x1 - x2 < FLT_EPSILON)
			{
				return (x1 + x2) / 2.0;
			}

			if (y1 > y2)
			{
				b = x1;

				x1 = x2;
				y1 = y2;

				x2 = a + (b - a) * (1.0 - rate);
				y2 = f(x2) + Pow(Min(0.0, g(x2)), 2.0) / FLT_EPSILON;
			}
			else
			{
				a = x2;

				x2 = x1;
				y2 = y1;

				x1 = a + (b - a) * rate;
				y1 = f(x1) + Pow(Min(0.0, g(x1)), 2.0) / FLT_EPSILON;
			}
		}
	}

private: // Static Function
	static double F(double x)
	{
		return Exp(-x) * x;
	}

	static double G(double x)
	{
		return x - 2.0;
	}

private: // Data Field
	std::vector<double> mIterations;
	std::vector<double> mLossValues;

private: // State Field
	bool mSolveValue = false;
	bool mPlotLoss = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H05_1_P02", createFunc, castFunc, "Project")
			.RegisterField("fExpression", &H05_1_P02::fExpression, "string", "ReadOnly")
			.RegisterField("gExpression", &H05_1_P02::gExpression, "string", "ReadOnly")
			.RegisterField("a", &H05_1_P02::a, "double", "Space(10)")
			.RegisterField("b", &H05_1_P02::b, "double");
	}
};  RCLASS_END(H05_1_P02)
