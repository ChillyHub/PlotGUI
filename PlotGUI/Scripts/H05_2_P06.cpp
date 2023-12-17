#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H05_2_P06 : public Script
{
public: // Inspector Field
	std::string fExpression = " f(x1, x2) = 10(x2 - x1^2)^2 + (1 - x1)^2";

private: // Custom Struct

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		if (mSolveValue)
		{
			Threads::Run([&]()
				{
					double minX1 = 0.0;
					double minX2 = 0.0;

					MinimizationGradientSearch(F, Fl1, Fl2, minX1, minX2);
					double minY = F(minX1, minX2);

					Console::Log("======================================================");
					Console::Log("Function %s", fExpression.c_str());
					Console::Log("When x = (%f, %f), min value = %f", minX1, minX2, minY);
					Console::Log("======================================================");
					Console::Log("\n");
				});
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 05: practice06");
		ImGui::TextWrapped("Write a program to find the minimum of a function of several variables f(x) using the gradient \
method when choosing iteration parameters from the minimum of a function of one variable, which is found by the golden section method. \
Illustrate how the program works when minimizing a function. \n\
        f(x1, x2) = 10(x2 - x1^2)^2 + (1 - x1)^2 \n\
Draw a convergence graph of the method");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H05_2_P06");

		ImGui::Spacing();

		mSolveValue = ImGui::Button("Solve Minimization", { -1, 0 });
		mPlotLoss |= mSolveValue;
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::PlotColorMap2DG<double>("Function", "x1", "x2", F, 400, 400, fExpression.c_str(), desc1);

		if (mPlotLoss)
		{
			PlotDescriptor desc2;
			desc2.axisFlags |= ImPlotAxisFlags_AutoFit;
		
			Plot::Plot1("Gradient Search Minimization Loss", "Iteration Count", "Loss Value",
				mIterations.data(), mLossValues.data(), mIterations.size(), "Gradient Search Method", desc2);
		}
	}

private: // Class Function
	void MinimizationGradientSearch(const std::function<double(double, double)>& f, 
		const std::function<double(double, double)>& fl1, 
		const std::function<double(double, double)>& fl2, double& x10, double& x20)
	{
		int i = 0;
		mIterations.clear();
		mLossValues.clear();

		while (true)
		{
			double h10 = -fl1(x10, x20);
			double h20 = -fl2(x10, x20);

			double delta = MinimizationGoldenSection([&](double alpha)
				{
					return f(x10 + alpha * h10, x20 + alpha * h20);
				}, 0.0, 1.0);

			double d1 = delta * h10;
			double d2 = delta * h20;

			double loss = Sqrt(d1 * d1 + d2 * d2);

			x10 += d1;
			x20 += d2;

			mIterations.push_back(static_cast<double>(++i));
			mLossValues.push_back(loss);

			if (loss < DBL_EPSILON)
			{
				return;
			}
		}
	}

	double MinimizationGoldenSection(const std::function<double(double)>& f, double a, double b)
	{
		const double rate = (Sqrt(5.0) - 1.0) / 2.0;

		double x1 = a + (b - a) * rate;
		double y1 = f(x1);

		double x2 = a + (b - a) * (1.0 - rate);
		double y2 = f(x2);

		int i = 0;

		while (true)
		{
			if (x1 - x2 < DBL_EPSILON)
			{
				return (x1 + x2) / 2.0;
			}

			if (y1 > y2)
			{
				b = x1;

				x1 = x2;
				y1 = y2;

				x2 = a + (b - a) * (1.0 - rate);
				y2 = f(x2);
			}
			else
			{
				a = x2;

				x2 = x1;
				y2 = y1;

				x1 = a + (b - a) * rate;
				y1 = f(x1);
			}
		}
	}

private: // Static Function
	static double F(double x1, double x2)
	{
		return 10.0 * Pow(x2 - x1 * x1, 2.0) + (1.0 - x1) * (1.0 - x1);
	}

	static double Fl1(double x1, double x2)
	{
		return -40.0 * (x2 - x1 * x1) * x1 - 2.0 * (1.0 - x1);
	}

	static double Fl2(double x1, double x2)
	{
		return 20.0 * (x2 - x1 * x1);
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
		Registry::RegisterClassDe("H05_2_P06", createFunc, castFunc, "Project")
			.RegisterField("fExpression", &H05_2_P06::fExpression, "string", "ReadOnly");
	}
};  RCLASS_END(H05_2_P06)
