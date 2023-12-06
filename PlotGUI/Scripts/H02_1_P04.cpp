#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;

RCLASS H02_1_P04 : public Script
{
public:
	int n = 10;
	float alpha = 0.5;

public:
	void Start() override
	{
		
	}

	void Update() override
	{
		if (!mCalculate)
		{
			return;
		}

		InitMatrixValue();

		int count = Interate();

		Console::Log("=======================================");
		Console::Log("When n = %d and alpha = %f", n, alpha);
		Console::Log("Interate count is %d", count);
		Console::Log("=======================================");
		Console::Log("\n");
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

		mCalculate = ImGui::Button("Calculate!", ImVec2(-1.0f, 0.0f));
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;
		desc.lineFlags |= ImPlotLineFlags_SkipNaN;

		// Plot::Plot1("Calculate Time of different dimensions", "dim", "time(ms)", dims.data(), times.data(), times.size(), "Time", desc);
	}

private:
	void InitMatrixValue()
	{
		mA = Math::DMatX(0.0, n, n);

		for (int i = 0; i < n; i++)
		{
			mA[i][i] = 2.0;
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = i + 1; j < n; j++)
			{
				mA[j][i] = -1.0 - alpha;
			}
		}

		for (int i = 1; i < n; i++)
		{
			for (int j = i - 1; j < n; j++)
			{
				mA[j][i] = -1.0 + alpha;
			}
		}

		mF = Math::DVecX(n, 0.0);
		mF[0] = 1 - alpha;
		mF[n - 1] = 1 + alpha;

		mX = Math::DVecX(n, 1.0);
	}

	int Interate()
	{
		int count = 0;
		double r0 = Math::Norm(mA * mX - mF);
		double r = r0;

		while (r > r0 * FLT_EPSILON)
		{
			mX = SeidelNextX(mX);
			count++;
		}

		return count;
	}

	Math::DVecX SeidelNextX(const Math::DVecX x)
	{
		Math::DMatX B(0.0, n, n);
		Math::DMatX E(1.0, n, n);

		for (int j = 0; j < n; j++)
		{
			for (int i = j; i < n; i++)
			{
				B[j][i] = mA[j][i];
			}
		}

		Math::DMatX B_inv = Math::Inverse(B);
		Math::DMatX S = E - B_inv * mA;

		return S * x + B_inv * mF;
	}

private:
	Math::DMatX mA;
	Math::DVecX mF;
	Math::DVecX mX;

private:
	bool mCalculate = false;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H02_1_P04", createFunc, castFunc, "Project")
			.RegisterField("n", &H02_1_P04::n, "int")
			.RegisterField("alpha", &H02_1_P04::alpha, "float", "Range(0.0,1.0)");
	}
};  RCLASS_END(H02_1_P04)
