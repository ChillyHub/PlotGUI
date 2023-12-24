#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H08_1_P01 : public Script
{
public: // Inspector Field
	int n = 50;
	double u0 = 1.0;
	double duDt0 = 0.0;
	double a = 0.0;
	double b = 4.0 * PI;

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

	}

	void Update() override
	{
		const std::vector<std::function<double(double, const DVecX&)>> functions = { DuDt, D2uDt2 };

		std::vector<DVecX> result;
		{
			Profile("Runge Kutta 4");
			result = RungeKutta4(functions, { u0, duDt0 }, a, b, n);
		}

		mCoefficients = GetNaturalCubicSplineCoefficients(result[0], result[1]);

		if (mShowProfile)
		{
			Console::Log("======================================================");
			Console::Log("Runge Kutta 4 Method: ");
			Console::Log("When n = %d, use time %f (ms)", n, Profiler::GetProfileInfo("Runge Kutta 4").deltaTime);
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

		ImGui::TextWrapped("Work 08: practice01");
		ImGui::TextWrapped("Write a program to numerically solve the Cauchy problem for a system of ordinary differential equations \
using the fourth-order explicit Runge-Kutta method. \n\
Demonstrate the functionality of this program in solving the Cauchy problem.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H08_1_P01");

		ImGui::Spacing();

		mShowProfile = ImGui::Button("Show Profile", { -1, 0 });
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::Plot1G<double>("Solve Differential Equations -- Explicit Runge-Kutta 4th", "", "", 400,
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mCoefficients);
			}, "Solve u(t)",
			mCoefficients.x.data(), mCoefficients.y.data(), mCoefficients.count, desc1);
	}

private: // Class Function

private: // Static Function
	static double D2uDt2(double t, const DVecX& u)
	{
		return -Sin(u[0]);
	}

	static double DuDt(double t, const DVecX& u)
	{
		return u[1];
	}

	static std::vector<DVecX> RungeKutta4(
		const std::vector<std::function<double(double, const DVecX&)>>& functions,
		const DVecX& initialYs, double start, double end, int n)
	{
		double h = (end - start) / n;
		int yCount = static_cast<int>(initialYs.length());

		std::vector<DVecX> result(yCount + 1, DVecX(n + 1, 0.0));

		DVecX ts(n + 1, 0.0);
		DMatX ys(0.0, yCount, n + 1);

		ts[0] = start;
		ys[0] = initialYs;

		for (int i = 0; i < n; ++i)
		{
			double t = start + (i + 1) * h;

			ts[i + 1] = t;

			for (int j = 0; j < yCount; ++j)
			{
				double s1 = functions[j](t, ys[i]);
				double s2 = functions[j](t + h / 2.0, ys[i] + h * s1 / 2.0);
				double s3 = functions[j](t + h / 2.0, ys[i] + h * s2 / 2.0);
				double s4 = functions[j](t + h, ys[i] + h * s3);

				ys[i + 1][j] = ys[i][j] + h * (s1 + 2.0 * s2 + 2.0 * s3 + s4) / 6.0;
			}
		}

		ys = Transpose(ys);

		result[0] = ts;
		for (int i = 0; i < yCount; ++i)
		{
			result[i + 1] = ys[i];
		}

		return result;
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

private: // State Field
	bool mShowProfile = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H08_1_P01", createFunc, castFunc, "Project")
			.RegisterField("n", &H08_1_P01::n, "int", "Range(1,400)")
			.RegisterField("u0", &H08_1_P01::u0, "double")
			.RegisterField("duDt0", &H08_1_P01::duDt0, "double")
			.RegisterField("a", &H08_1_P01::a, "double")
			.RegisterField("b", &H08_1_P01::b, "double");
	}
};  RCLASS_END(H08_1_P01)
