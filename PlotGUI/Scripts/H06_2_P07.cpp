#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H06_2_P07 : public Script
{
public: // Inspector Field
	std::string gridFunction = " f(x) = 1 - cos(x)";

	int n = 10;
	int m = 3;

private: // Custom Struct
	struct PolynomialCoefficients
	{
		int count;
		DVecX coef;

		PolynomialCoefficients() : count(0) {}

		PolynomialCoefficients(int count)
			: count(count), coef(count)
		{

		}
	};

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		auto x = GetX(n);

		DVecX y(x.length());
		for (int i = 0; i < x.length(); ++i)
		{
			y[i] = GridFunction(x[i]);
		}

		mCoefficients = GetPolynomialCoefficients(x, y, m);

		if (mCalcCompare)
		{
			Threads::Run([&]()
			{
				auto xx = GetX(10);

				DVecX yy(xx.length());
				for (int i = 0; i < xx.length(); ++i)
				{
					yy[i] = GridFunction(xx[i]);
				}

				mCoefficientsM1 = GetPolynomialCoefficients(xx, yy, 1);
				mCoefficientsM2 = GetPolynomialCoefficients(xx, yy, 2);
				mCoefficientsM3 = GetPolynomialCoefficients(xx, yy, 3);

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

		ImGui::TextWrapped("Work 06: practice07");
		ImGui::TextWrapped("Write a program to approximate the grid function y_i \n\
		i = 0,1, ..., n \n\
polynomial p_m(x) = c0 + c1x + c2x ^ 2 + ... + c_m*x ^ m, m < n by the least squares method. \
Illustrate the performance of the program using an example of approximation \n\
		y_i = 1 - cos(x_i) \n\
		x_i = ih, h = 1 / n, i = 0,1,...,n \n\
for n = 10 and m = 1; 2; 3. \n\
Save the obtained coefficients of the interpolation polynomial in a file. \n\
Draw graphs of the approximation polynomial, the desired function and the given values.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H06_2_P07");

		ImGui::Spacing();

		mCalcCompare = ImGui::Button("Show Compare Plots", { -1, 0 });

		mSaveFile = ImGui::Button("Save coeffocients", { -1, 0 });
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		auto x = GetX(n);

		DVecX y(x.length());
		for (int i = 0; i < x.length(); ++i)
		{
			y[i] = GridFunction(x[i]);
		}

		Plot::Plot1G<double>("Interpolation Use Least Squares Method", "", "", 400,
			[&](double x) -> double
			{
				return PolynomialFunction(x, mCoefficients);
			}, "Polynomial", 
			x.data(), y.data(), x.length(), desc1);

		if (mPlotCompare)
		{
			PlotDescriptor desc2;
			desc2.plotFlags |= ImPlotFlags_Equal;
			desc2.subplotFlags |= ImPlotSubplotFlags_LinkAllX | ImPlotSubplotFlags_LinkAllY;

			auto xx = GetX(10);

			DVecX yy(xx.length());
			for (int i = 0; i < xx.length(); ++i)
			{
				yy[i] = GridFunction(xx[i]);
			}

			Plot::Plot3G<double>("Interpolation Use Least Squares Method Compare", "", "", 400,
				[&](double x) -> double
				{
					return PolynomialFunction(x, mCoefficientsM1);
				}, "Polynomial n = 10, m = 1",
				[&](double x) -> double
				{
					return PolynomialFunction(x, mCoefficientsM2);
				}, "Polynomial n = 10, m = 2",
				[&](double x) -> double
				{
					return PolynomialFunction(x, mCoefficientsM3);
				}, "Polynomial n = 10, m = 3",
				xx.data(), yy.data(), xx.length(), desc2);
		}
	}

private: // Class Function
	void SaveCoefficients(const std::string& filename, const PolynomialCoefficients& coef)
	{
		json j;

		std::string path = "Resources/H06_2_P07/";

		Serialization::ReadJson(path + filename, j);

		std::vector<double> data(coef.count);
		for (int i = 0; i < coef.count; ++i)
		{
			data[i] = coef.coef[i];
		}

		j["coefficients"].push_back(
			{
				{ "count", coef.count },
				{ "coef", data }
			});

		if (Serialization::SaveJson(path, filename, j))
		{
			Console::LogInfo("Save file succeed");
		}
	}

private: // Static Function
	static double GridFunction(double x)
	{
		return 1.0 - Cos(x);
	}

	static double PolynomialFunction(double x, const PolynomialCoefficients& coef)
	{
		double y = 0.0;

		for (int i = 0; i < coef.count; ++i)
		{
			y += coef.coef[i] * Pow(x, static_cast<float>(i));
		}

		return y;
	}

	static PolynomialCoefficients GetPolynomialCoefficients(const DVecX& x, const DVecX& y, int m)
	{
		int n = x.length();

		PolynomialCoefficients coef(m + 1);

		DMatX A(n, m + 1);

		for (int j = 0; j < m + 1; ++j)
		{
			for (int i = 0; i < n; ++i)
			{
				A[j][i] = Pow(x[i], static_cast<float>(j));
			}
		}

		DMatX AT = Transpose(A);

		coef.coef = Solve(AT * A, AT * y);

		return coef;
	}

	static DVecX GetX(int nn)
	{
		DVecX x(nn + 1);
		double h = 1.0 / nn;

		for (int i = 0; i <= nn; ++i)
		{
			x[i] = i * h;
		}

		return x;
	}

private: // Data Field
	PolynomialCoefficients mCoefficients;
	PolynomialCoefficients mCoefficientsM1;
	PolynomialCoefficients mCoefficientsM2;
	PolynomialCoefficients mCoefficientsM3;

private: // State Field
	bool mCalcCompare = false;
	bool mPlotCompare = false;
	bool mSaveFile = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H06_2_P07", createFunc, castFunc, "Project")
			.RegisterField("gridFunction", &H06_2_P07::gridFunction, "string", "ReadOnly")
			.RegisterField("n", &H06_2_P07::n, "int", "Range(1,40) Space(10)")
			.RegisterField("m", &H06_2_P07::m, "int", "Range(1,10)");
	}
};  RCLASS_END(H06_2_P07)
