#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H07_1_P01 : public Script
{
public: // Inspector Field
	int n = 10;

private: // Custom Struct
	struct CubicSplineCoefficients
	{
		int count;
		std::vector<double> x;
		std::vector<double> y;
		std::vector<double> b;
		std::vector<double> c;
		std::vector<double> d;

		CubicSplineCoefficients() : count(0) {}

		CubicSplineCoefficients(int count)
			: count(count), x(count), y(count), b(count), c(count), d(count)
		{

		}
	};

public: // Override Func
	void Start() override
	{
		mSplitNs.assign(40, 0.0);
		mResiduals.assign(40, 0.0);

		for (int i = 0; i < 40; ++i)
		{
			mSplitNs[i] = i + 1;
		}
	}

	void Update() override
	{
		DVecX x, y;
		SolveIntegralEquation(0.0, 1.0, 0.0, 1.0, n, x, y);

		mCoefficients = GetNaturalCubicSplineCoefficients(x, y);

		if (mCalcCompare)
		{
			Threads::Run([&]()
			{
				for (int i = 0; i < 40; ++i)
				{
					DVecX xx, yy;
					SolveIntegralEquation(0.0, 1.0, 0.0, 1.0, static_cast<int>(mSplitNs[i]), xx, yy);

					mResiduals[i] = SolveResidual(0.0, 1.0, static_cast<int>(mSplitNs[i]), xx, yy);

					mPlotCompare = true;
				}

				Console::Log("Finish Calculate");
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

		ImGui::TextWrapped("Work 07: practice01");
		ImGui::TextWrapped("Write a program for the numerical solution of the Fredholm integral equation of the second kind by the \
quadrature method using Simpson's quadrature formula with a uniform division of integration intervals. \
Use this program to approximately solve an integral equation. \n\
Investigate the dependence of the accuracy of the numerical solution on the number of partial segments.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H07_1_P01");

		ImGui::Spacing();

		mCalcCompare = ImGui::Button("Calculate Residual", { -1, 0 });
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::Plot1G<double>("Solve Integral Equation -- Simpson", "", "", 400,
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mCoefficients);
			}, "Solve f(x)",
			mCoefficients.x.data(), mCoefficients.y.data(), mCoefficients.count, desc1);

		if (mPlotCompare)
		{
			PlotDescriptor desc2;
			desc2.axisFlags |= ImPlotAxisFlags_AutoFit;

			Plot::Plot1("Residual of Solve", "split n", "residual",
				mSplitNs.data(), mResiduals.data(), mSplitNs.size(), "Simpson Method", desc2);
		}
	}

private: // Class Function

private: // Static Function
	static double K(double x, double s)
	{
		return x - s;
	}

	static double F(double x)
	{
		return 3.0 - 2.0 * x;
	}

	static void SolveIntegralEquation(double ax, double bx, double as, double bs, int n, DVecX& x, DVecX& y)
	{
		x = DVecX(n + 1);

		DVecX b(n + 1);
		DMatX A(1.0, n + 1, n + 1);

		double hx = (bx - ax) / n;
		double hs = (bs - as) / n;

		for (int i = 0; i <= n; ++i)
		{
			x[i] = ax + hx * i;

			for (int j = 0; j <= n; ++j)
			{
				double s = as + hs * j;
				double w = (j == 0 || j == n) ? hs / 3 : (j % 2 == 0) ? 2 * hs / 3 : 4 * hs / 3;
				A[j][i] -= w * K(x[i], s);
			}

			b[i] = F(x[i]);
		}

		y = Solve(A, b);
	}

	static double SolveResidual(double as, double bs, int n, const DVecX& x, const DVecX& y)
	{
		double hs = (bs - as) / n;

		double residual = 0.0;

		for (int i = 0; i <= n; ++i)
		{
			double sum = y[i];

			for (int j = 0; j <= n; ++j)
			{
				double s = as + hs * j;
				double w = (j == 0 || j == n) ? hs / 3 : (j % 2 == 0) ? 2 * hs / 3 : 4 * hs / 3;

				sum -= w * K(x[i], s) * y[j];
			}

			residual += Pow(sum - F(x[i]), 2.0f);
		}

		return Sqrt(residual);
	}

	static double CubicSplineFunction(double x, const CubicSplineCoefficients& coef)
	{
		if (coef.count == 0)
		{
			return 0.0;
		}

		int i = 0;

		for (; i < coef.count - 2; ++i)
		{
			if (x < coef.x[i + 1])
			{
				break;
			}
		}

		double dx = x - coef.x[i];

		return coef.y[i] + coef.b[i] * dx + coef.c[i] * dx * dx + coef.d[i] * dx * dx * dx;
	}

	static CubicSplineCoefficients GetNaturalCubicSplineCoefficients(const DVecX& x, const DVecX& y)
	{
		int count = static_cast<int>(x.length());

		CubicSplineCoefficients coef(count);

		for (int i = 0; i < count; ++i)
		{
			coef.x[i] = x[i];
			coef.y[i] = y[i];
		}

		std::vector<double> h(count - 1);
		std::vector<double> alpha(count - 1);

		for (int i = 0; i < count - 1; ++i)
		{
			h[i] = x[i + 1] - x[i];
		}

		for (int i = 1; i < count - 1; ++i)
		{
			alpha[i] = 3.0 * (y[i + 1] - y[i]) / h[i] - 3.0 * (y[i] - y[i - 1]) / h[i - 1];
		}

		std::vector<double> l(count);
		std::vector<double> mu(count);
		std::vector<double> z(count);

		l[0] = 1.0;
		mu[0] = 0.0;
		z[0] = 0.0;

		for (int i = 1; i < count - 1; ++i)
		{
			l[i] = 2.0 * (h[i] + h[i - 1]) - h[i - 1] * mu[i - 1];
			mu[i] = h[i] / l[i];
			z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
		}

		l[count - 1] = 1.0;
		z[count - 1] = 0.0;

		coef.c[count - 1] = 0.0;

		for (int i = count - 2; i >= 0; --i)
		{
			coef.c[i] = z[i] - mu[i] * coef.c[i + 1];
			coef.d[i] = (coef.c[i + 1] - coef.c[i]) / (3.0 * h[i]);
			coef.b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (2.0 * coef.c[i] + coef.c[i + 1]) / 3.0;
		}

		return coef;
	}

private: // Data Field
	CubicSplineCoefficients mCoefficients;
	std::vector<double> mSplitNs;
	std::vector<double> mResiduals;

private: // State Field
	bool mCalcCompare = false;
	bool mPlotCompare = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H07_1_P01", createFunc, castFunc, "Project")
			.RegisterField("n", &H07_1_P01::n, "int", "Range(1,40)");
	}
};  RCLASS_END(H07_1_P01)
