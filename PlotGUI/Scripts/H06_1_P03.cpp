#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H06_1_P03 : public Script
{
public: // Inspector Field
	std::string rungeFunction = " f(x) = 1 / (1 + 25x^2)";

	double a = -1.0;
	double b = 1.0;
	int n = 4;

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
		auto x = GetEquidistantNodes(a, b, n);

		std::vector<double> y(x.size());
		for (int i = 0; i < x.size(); ++i)
		{
			y[i] = RungeFunction(x[i]);
		}

		mCoefficients = GetNaturalCubicSplineCoefficients(x, y);

		if (mCalcCompare)
		{
			Threads::Run([&]()
			{
				for (int i = 0; i < 4; ++i)
				{
					auto x = GetEquidistantNodes(-1.0, 1.0, mSplitNs[i]);

					std::vector<double> y(x.size());
					for (int j = 0; j < x.size(); ++j)
					{
						y[j] = RungeFunction(x[j]);
					}

					mCoefficientsEquidistant[i] = GetNaturalCubicSplineCoefficients(x, y);
				}

				for (int i = 0; i < 4; ++i)
				{
					auto x = GetChebyshevNodes(-1.0, 1.0, mSplitNs[i]);

					std::vector<double> y(x.size());
					for (int j = 0; j < x.size(); ++j)
					{
						y[j] = RungeFunction(x[j]);
					}

					mCoefficientsChebyshev[i] = GetNaturalCubicSplineCoefficients(x, y);
				}

				mPlotCompare = true;

				Console::Log("Finish Calculate");
				Console::Log("\n");
			});
		}

		if (mSaveFile)
		{
			SaveCoefficients("Coefficients.json", mCoefficients);
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 06: practice03");
		ImGui::TextWrapped("Write a program to interpolate data using a natural interpolating cubic spline. Use this program to interpolate data for the Runge function \n\
		1. in equally spaced nodes \n\
		2. at nodes located at the zeros of the Chebyshev polynomial. \n\
on the interval [-1, 1] for n = 4, 6, 10, 20. \n\
Save the resulting interpolation spline coefficients in a file. \n\
Draw graphs of the interpolation spline, the desired function and the given values.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H06_1_P03");

		ImGui::Spacing();

		mCalcCompare = ImGui::Button("Show Compare Plots", { -1, 0 });

		mSaveFile = ImGui::Button("Save coeffocients", { -1, 0 });
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::Plot2G<double>("Interpolation of Runge Function", "", "", 400,
			RungeFunction, "Runge Function",
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mCoefficients);
			}, "Natural Cubic Spline", desc1);

		if (mPlotCompare)
		{
			PlotDescriptor desc2;
			desc2.plotFlags |= ImPlotFlags_Equal;
			desc2.subplotFlags |= ImPlotSubplotFlags_LinkAllX | ImPlotSubplotFlags_LinkAllY;

			for (int i = 0; i < 4; ++i)
			{
				std::string title = "Interpolation when n = ";
				title += std::to_string(mSplitNs[i]);

				Plot::Subplots2_2G2G<double>(title.c_str(),
					"Equidistant Split", "", "", 400,
					// Plot1
					RungeFunction, "Runge Function",
					[&](double x) -> double
					{
						return CubicSplineFunction(x, mCoefficientsEquidistant[i]);
					}, "Natural Cubic Spline",
					mCoefficientsEquidistant[i].x.data(), mCoefficientsEquidistant[i].y.data(), mCoefficientsEquidistant[i].count,
					// Plot2
					"Chebyshev Split", "", "", 400,
					RungeFunction, "Runge Function",
					[&](double x) -> double
					{
						return CubicSplineFunction(x, mCoefficientsChebyshev[i]);
					}, "Natural Cubic Spline", 
					mCoefficientsChebyshev[i].x.data(), mCoefficientsChebyshev[i].y.data(), mCoefficientsChebyshev[i].count,
					// Desc
					desc2);
			}
		}
	}

private: // Class Function
	void SaveCoefficients(const std::string& filename, const CubicSplineCoefficients& coef)
	{
		json j;

		std::string path = "Resources/H06_1_P03/";

		Serialization::ReadJson(path + filename, j);

		j["coefficients"].push_back(
			{
				{ "count", coef.count },
				{ "x", coef.x },
				{ "y", coef.y },
				{ "b", coef.b },
				{ "c", coef.c },
				{ "d", coef.d },
			});

		if (Serialization::SaveJson(path, filename, j))
		{
			Console::LogInfo("Save file succeed");
		}
	}

private: // Static Function
	static double RungeFunction(double x)
	{
		return 1.0 / (1.0 + 25.0 * x * x);
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

	static CubicSplineCoefficients GetNaturalCubicSplineCoefficients(std::vector<double> x, std::vector<double> y)
	{
		int count = static_cast<int>(x.size());

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

	static std::vector<double> GetEquidistantNodes(double a, double b, int count)
	{
		std::vector<double> nodes(count);

		double h = (b - a) / (count - 1);

		for (int i = 0; i < count; ++i)
		{
			nodes[i] = a + i * h;
		}

		return nodes;
	}

	static std::vector<double> GetChebyshevNodes(double a, double b, int count)
	{
		std::vector<double> nodes(count);

		for (int i = 0; i < count; ++i)
		{
			nodes[i] = -(a + b) / 2.0 - (b - a) / 2.0 * Cos((2.0 * i + 1.0) * PI / (2.0 * count));
		}

		return nodes;
	}

private: // Data Field
	CubicSplineCoefficients mCoefficients;
	CubicSplineCoefficients mCoefficientsEquidistant[4];
	CubicSplineCoefficients mCoefficientsChebyshev[4];
	int mSplitNs[4] = { 4, 6, 10, 20 };

private: // State Field
	bool mCalcCompare = false;
	bool mPlotCompare = false;
	bool mSaveFile = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H06_1_P03", createFunc, castFunc, "Project")
			.RegisterField("rungeFunction", &H06_1_P03::rungeFunction, "string", "ReadOnly")
			.RegisterField("a", &H06_1_P03::a, "double", "Space(10)")
			.RegisterField("b", &H06_1_P03::b, "double")
			.RegisterField("n", &H06_1_P03::n, "int", "Range(1,40)");
	}
};  RCLASS_END(H06_1_P03)
