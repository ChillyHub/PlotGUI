#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H04_1_P09 : public Script
{
public: // Inspector Field
	std::string f1Expression = " f1 = 3 / ((x - 1)^2 + 1)";
	std::string f2Expression = " f2 = sqrt(x + 0.5)";
	std::string f3Expression = " f3 = exp(-x)";
	std::string f1Derivative = " f1' = -6(x-1) / ((x-1)^4 + 2(x-1)^2 + 1)";
	std::string f2Derivative = " f2' = 1 / (2 * sqrt(x + 0.5))";
	std::string f3Derivative = " f3' = -exp(-x)";
	std::string f1SecondDerivative = " f1'' = (18(x-1)^4+12(x-1)^2-6)/((x-1)^4+2(x-1)^2+1)^2";
	std::string f2SecondDerivative = " f2'' = -1 / 4(x+0.5)^(1.5)";
	std::string f3SecondDerivative = " f3'' = exp(-x)";

	double eps1 = 0.001;
	double eps2 = 0.001;

private: // Custom Struct

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		if (mSolveRoot)
		{
			// Solve Root -----------------------------------------------------------------
			// ----------------------------------------------------------------------------
			mLossXBinary.clear();
			mLossXNewton.clear();
			mLossYBinary.clear();
			mLossYNewton.clear();

			double root12B = RootBinary(F1, F2, 1.6, 10.0, eps1);
			double root23B = RootBinary(F2, F3, 0.0, 10.0, eps1);
			double root31B = RootBinary(F3, F1, -0.26, 0.33, eps1);

			double root12N = RootNewton(F1, F2, F1D, F2D, F1SD, F2SD, 1.6, 10.0, eps1);
			double root23N = RootNewton(F2, F3, F2D, F3D, F2SD, F3SD, 0.0, 10.0, eps1);
			double root31N = RootNewton(F3, F1, F3D, F1D, F3SD, F1SD, -0.26, 0.33, eps1);

			for (double& n : mLossYBinary)
			{
				n = Sqrt(n);
			}

			for (double& n : mLossYNewton)
			{
				n = Sqrt(n);
			}

			mRoot12 = (root12B + root12N) * 0.5;
			mRoot23 = (root23B + root23N) * 0.5;
			mRoot31 = (root31B + root31N) * 0.5;

			double value12 = (F1(mRoot12) + F2(mRoot12)) * 0.5;
			double value23 = (F2(mRoot23) + F3(mRoot23)) * 0.5;
			double value31 = (F3(mRoot31) + F1(mRoot31)) * 0.5;

			mScatterX.clear();
			mScatterY.clear();
			mScatterX.push_back(mRoot12);
			mScatterX.push_back(mRoot23);
			mScatterX.push_back(mRoot31);
			mScatterY.push_back(value12);
			mScatterY.push_back(value23);
			mScatterY.push_back(value31);

			// Calculate Area -------------------------------------------------------------
			// ----------------------------------------------------------------------------
			mLossXRectangle.clear();
			mLossXTrapezoid.clear();
			mLossYRectangle.clear();
			mLossYTrapezoid.clear();

			double S1R = IntergralRectangle(F1, mScatterX[2], mScatterX[0], eps2);
			double S2R = IntergralRectangle(F2, mScatterX[1], mScatterX[0], eps2);
			double S3R = IntergralRectangle(F3, mScatterX[2], mScatterX[1], eps2);

			double S1T = IntergralTrapezoid(F1, mScatterX[2], mScatterX[0], eps2);
			double S2T = IntergralTrapezoid(F2, mScatterX[1], mScatterX[0], eps2);
			double S3T = IntergralTrapezoid(F3, mScatterX[2], mScatterX[1], eps2);

			for (double& n : mLossYRectangle)
			{
				n = Sqrt(n);
			}

			for (double& n : mLossYTrapezoid)
			{
				n = Sqrt(n);
			}

			double SR = S1R - S2R - S3R;
			double ST = S1T - S2T - S3T;

			Console::Log("======================================================");
			Console::Log("If f1(x) = f2(x), x = %f, y = %f", mRoot12, value12);
			Console::Log("If f2(x) = f3(x), x = %f, y = %f", mRoot23, value23);
			Console::Log("If f3(x) = f1(x), x = %f, y = %f", mRoot31, value31);
			Console::Log("======================================================");
			Console::Log("Area Use Rectangle S = %f", SR);
			Console::Log("Area Use Trapezoid S = %f", ST);
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

		ImGui::TextWrapped("Work 04: practice09");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H04_1_P09");

		ImGui::Spacing();

		mSolveRoot = ImGui::Button("Solve Root and Calculate Area", { -1, 0 });
		mPlotLoss |= mSolveRoot;
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;
		//desc.axisFlags |= ImPlotAxisFlags_AutoFit;

		Plot::PlotShade3G<double>("Function", "", "", 400, F1, f1Expression, F2, f2Expression, F3, f3Expression, 
			mScatterX.data(), mScatterY.data(), mScatterX.size(), desc1);

		if (mPlotLoss)
		{
			PlotDescriptor desc2;
			desc2.axisFlags |= ImPlotAxisFlags_AutoFit;

			Plot::Plot2("Root Loss", "Iteration Count", "Forward Loss", 
				mLossXBinary.data(), mLossYBinary.data(), mLossXBinary.size(), "Binary Method", 
				mLossXNewton.data(), mLossYNewton.data(), mLossXNewton.size(), "Newton Method", desc2);

			Plot::Plot2("Area Loss", "Iteration Count", "Forward Loss",
				mLossXRectangle.data(), mLossYRectangle.data(), mLossXRectangle.size(), "Rectangle Method",
				mLossXTrapezoid.data(), mLossYTrapezoid.data(), mLossXTrapezoid.size(), "Trapezoid Method", desc2);
		}
	}

private: // Class Function
	double RootBinary(const std::function<double(double)>& f, const std::function<double(double)>& g, double a, double b, double eps)
	{
		double i = 0.0;

		while ((b - a) / 2.0 > eps)
		{
			double c = (a + b) / 2.0;
			double fc = f(c) - g(c);
			double fa = f(a) - g(a);

			if (i < mLossXBinary.size())
			{
				mLossYBinary[i++] += Abs(fc) * Abs(fc);
			}
			else
			{
				mLossXBinary.push_back(++i);
				mLossYBinary.push_back(Abs(fc) * Abs(fc));
			}

			if (fc == 0.0)
			{
				return c;
			}

			if (fa * fc < 0.0)
			{
				b = c;
			}
			else
			{
				a = c;
			}
		}

		return (a + b) / 2.0;
	}

	double RootNewton(const std::function<double(double)>& f, const std::function<double(double)>& g, 
		const std::function<double(double)>& fl, const std::function<double(double)>& gl,
		const std::function<double(double)>& fll, const std::function<double(double)>& gll,
		double a, double b, double eps)
	{
		double i = 0.0;

		double c = (a + b) / 2.0;
		double FD = fl(c) - gl(c);
		double FSD = fll(c) - gll(c);

		double x0;
		if (FD * FSD > 0)
		{
			x0 = b;
		}
		else
		{
			x0 = a;
		}

		while (true)
		{
			double fx0 = f(x0) - g(x0);
			double fDx0 = fl(x0) - gl(x0);

			double delta = fx0 / fDx0;

			if (isnan(delta))
			{
				throw std::runtime_error("Have nan number when solving. Change the range.");
			}

			if (Abs(delta) <= eps)
			{
				break;
			}

			x0 = x0 - delta;

			double v = Abs(f(x0) - g(x0));

			if (i < mLossXNewton.size())
			{
				mLossYNewton[i++] += v * v;
			}
			else
			{
				mLossXNewton.push_back(++i);
				mLossYNewton.push_back(v * v);
			}
		}

		return x0;
	}

	double IntergralRectangle(const std::function<double(double)>& f, double a, double b, double eps)
	{
		double n = 1.0;

		double h = (b - a) / n;
		double F = 0.0;

		for (int i = 0; i < n; ++i)
		{
			F += f(a + (static_cast<double>(i) + 0.5) * h);
		}

		double In = F * h;
		double I2n = 0.0;

		double i = 0.0;

		while (true)
		{
			n *= 2.0;

			h = (b - a) / n;
			F = 0.0;

			for (int i = 0; i < n; ++i)
			{
				F += f(a + (static_cast<double>(i) + 0.5) * h);
			}

			I2n = F * h;

			double loss = Abs(In - I2n) * (1.0 / 3.0);

			if (i < mLossXRectangle.size())
			{
				mLossYRectangle[i++] += loss * loss;
			}
			else
			{
				mLossXRectangle.push_back(++i);
				mLossYRectangle.push_back(loss * loss);
			}

			if (loss < eps)
			{
				break;
			}

			In = I2n;
		}

		return I2n;
	}

	double IntergralTrapezoid(const std::function<double(double)>& f, double a, double b, double eps)
	{
		double n = 1.0;

		double h = (b - a) / n;
		double F = 0.0;

		for (int i = 0; i <= n; ++i)
		{
			double fac = (i == 0 || i == n) ? 0.5 : 1.0;
			F += fac * f(a + h * i);
		}

		double In = F * h;
		double I2n = 0.0;

		double i = 0.0;

		while (true)
		{
			n *= 2.0;

			h = (b - a) / n;
			F = 0.0;

			for (int i = 0; i <= n; ++i)
			{
				double fac = (i == 0 || i == n) ? 0.5 : 1.0;
				F += fac * f(a + h * i);
			}

			I2n = F * h;

			double loss = Abs(In - I2n) * (1.0 / 3.0);

			if (i < mLossXTrapezoid.size())
			{
				mLossYTrapezoid[i++] += loss * loss;
			}
			else
			{
				mLossXTrapezoid.push_back(++i);
				mLossYTrapezoid.push_back(loss * loss);
			}

			if (loss < eps)
			{
				break;
			}

			In = I2n;
		}

		return I2n;
	}

private: // Static Function
	static double F1(double x)
	{
		return 3.0 / (Pow(x - 1.0, 2.0) + 1);
	}

	static double F2(double x)
	{
		return Sqrt(x + 0.5);
	}

	static double F3(double x)
	{
		return Exp(-x);
	}

	static double F1D(double x)
	{
		double t = x - 1.0;
		double t2 = t * t;
		double t4 = t2 * t2;
		return -6.0 * t / (t4 + 2.0 * t2 + 1.0);
	}

	static double F2D(double x)
	{
		return 1.0 / (2.0 * Sqrt(x + 0.5));
	}

	static double F3D(double x)
	{
		return -Exp(-x);
	}

	static double F1SD(double x)
	{
		double t = x - 1.0;
		double t2 = t * t;
		double t4 = t2 * t2;
		double t6 = t4 * t2;
		double t8 = t4 * t4;
		return (18.0 * t4 + 12.0 * t2 - 6.0) / (t8 + 4.0 * t6 + 6.0 * t4 + 4.0 * t2 + 1.0);
	}

	static double F2SD(double x)
	{
		return -1.0 / (4.0 * Sqrt(Pow(x + 0.5, 3.0)));
	}

	static double F3SD(double x)
	{
		return Exp(-x);
	}

private: // Data Field
	double mRoot12 = 0.0;
	double mRoot23 = 0.0;
	double mRoot31 = 0.0;
	std::vector<double> mScatterX;
	std::vector<double> mScatterY;
	std::vector<double> mLossXBinary;
	std::vector<double> mLossYBinary;
	std::vector<double> mLossXNewton;
	std::vector<double> mLossYNewton;
	std::vector<double> mLossXRectangle;
	std::vector<double> mLossYRectangle;
	std::vector<double> mLossXTrapezoid;
	std::vector<double> mLossYTrapezoid;

private: // State Field
	bool mSolveRoot = false;
	bool mPlotLoss = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H04_1_P09", createFunc, castFunc, "Project")
			.RegisterField("f1Expression", &H04_1_P09::f1Expression, "string", "ReadOnly")
			.RegisterField("f2Expression", &H04_1_P09::f2Expression, "string", "ReadOnly")
			.RegisterField("f3Expression", &H04_1_P09::f3Expression, "string", "ReadOnly")
			.RegisterField("f1Derivative", &H04_1_P09::f1Derivative, "string", "ReadOnly Space(1)")
			.RegisterField("f2Derivative", &H04_1_P09::f2Derivative, "string", "ReadOnly")
			.RegisterField("f3Derivative", &H04_1_P09::f3Derivative, "string", "ReadOnly")
			.RegisterField("f1SecondDerivative", &H04_1_P09::f1SecondDerivative, "string", "ReadOnly Space(1)")
			.RegisterField("f2SecondDerivative", &H04_1_P09::f2SecondDerivative, "string", "ReadOnly")
			.RegisterField("f3SecondDerivative", &H04_1_P09::f3SecondDerivative, "string", "ReadOnly")
			.RegisterField("eps1", &H04_1_P09::eps1, "double", "Range(0.0001,0.01) Space(10)")
			.RegisterField("eps2", &H04_1_P09::eps2, "double", "Range(0.0001,0.01)");
	}
};  RCLASS_END(H04_1_P09)
