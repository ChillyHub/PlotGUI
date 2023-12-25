#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H10_1_P05 : public Script
{
public: // Inspector Field
	int n = 5;
	double kai = 1.0;

private: // Custom Struct

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		mIterationCounts.clear();
		mResiduals.clear();

		//SolveDifferenceMethod(0.0, 1.0, 0.0, 1.0, 0.0, n, kai, mX1, mX2, mY, mIterationCounts, mResiduals);

		//if (mComplete)
		//{
		//	mPlotCoefficients[0] = mCoefficients[0];
		//	mPlotCoefficients[1] = mCoefficients[1];
		//	mPlotCoefficients[2] = mCoefficients[2];
		//
		//	mComplete = false;
		//
		//	Threads::Run([&]()
		//	{
		//		const double kais[3] = { 0.01, 1.0, 100.0 };
		//
		//		for (int i = 0; i < 3; ++i)
		//		{
		//			double h = h0 / divs[i];
		//
		//			DVecX x, y;
		//			SolveDifferenceMethod(0.0001, l, miu1, miu2, h, x, y);
		//
		//			mCoefficients[i] = GetNaturalCubicSplineCoefficients(x, y);
		//		}
		//
		//		mComplete = true;
		//	});
		//}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 10: practice05");
		ImGui::TextWrapped("Write a program for the difference solution of boundary value problems \
using an exact difference scheme when using trapezoidal quadrature formulas to calculate the coefficients. \
Apply this program to solve a boundary value problem (for l = 1 and miu_1 = miu_2 = 0) with discontinuity coefficient k(x) and right-hand side f(x) = 0. \n\
Compare the approximate solution on a sequence of three nested grids h = h0, h0 / 2, h0 / 4. \
Draw the resulting solution and conduct computational experiments to study the rate of convergence of the difference scheme used.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H10_1_P05");
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;
		//desc1.axisFlags |= ImPlotAxisFlags_AutoFit;

		AxesLimits limits;
		limits.xMin = 0.0;
		limits.xMax = 1.0;
		limits.yMin = 0.0;
		limits.yMax = 1.0;

		int c = static_cast<int>(Sqrt(mY.length()));

		for (int i = 0; i < c / 2; ++i)
		{
			for (int j = 0; j < c; ++j)
			{
				double tmp = mY[i * c + j];
				mY[i * c + j] = mY[(c - 1 - i) * c + j];
				mY[(c - 1 - i) * c + j] = tmp;
			}
		}

		Plot::PlotColorMap2D("Solve Differential Equations -- Difference, Jacobi, ConjugateGradient Method", "x1", "x2",
			mY.data(), c, c, "u(x1, x2)", desc1, limits);
	}

private: // Class Function
	void SolveDifferenceMethod(double a1, double b1, double a2, double b2, double u0, int n, double kai, 
		DVecX& x1, DVecX& x2, DVecX& y, std::vector<double>& iterationCounts, std::vector<double>& residuals)
	{
		DMatX A;
		DVecX b;
		CoefficientsTrapezoidal(a1, b1, a2, b2, n, u0, kai, A, b);

		//bool is = IsPositiveDefinite(A);
		//
		//std::cout << IsPositiveDefinite(A) << A << b << std::endl;

		double h1 = (b1 - a1) / n;
		double h2 = (b2 - a2) / n;

		x1 = DVecX((n + 1) * (n + 1), 0.0);
		x2 = DVecX((n + 1) * (n + 1), 0.0);
		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= n; ++j)
			{
				x1[i * (n + 1) + j] = a1 + j * h1;
				x2[i * (n + 1) + j] = a2 + i * h2;
			}
		}

		y = DVecX((n + 1) * (n + 1), 0.0);
		IterateJacobi(A, b, y, iterationCounts, residuals);

		//y = DVecX((n + 1) * (n + 1), 0.0);
		//for (int i = 0; i <= n; ++i)
		//{
		//	for (int j = 0; j <= n; ++j)
		//	{
		//		if (i == 0 || i == n || j == 0 || j == n)
		//		{
		//			y[i * (n + 1) + j] = u0;
		//		}
		//		else
		//		{
		//			y[i * (n + 1) + j] = yy[(i - 1) * (n - 1) + j - 1];
		//		}
		//	}
		//}
	}

	void CoefficientsTrapezoidal(double a1, double b1, double a2, double b2, int n, double u0, double kai, DMatX& A, DVecX& b)
	{
		double h1 = (b1 - a1) / n;
		double h2 = (b2 - a2) / n;

		int c = n + 1;
		A = DMatX(0.0, c * c, c * c);
		b = DVecX(c * c, 0.0);

		for (int i = 1; i < c - 1; ++i)
		{
			for (int j = 1; j < c - 1; ++j)
			{
				double x1m = a1 + (j + 0) * h1;
				double x1p = a1 + (j + 1) * h1;
				double x2m = a2 + (i + 0) * h2;
				double x2p = a2 + (i + 1) * h2;

				double a1m = GetA1(x1m, x2m, h1, h2, [&](double x1, double x2) { return K(x1, x2, kai); });
				double a2m = GetA2(x1m, x2m, h1, h2, [&](double x1, double x2) { return K(x1, x2, kai); });
				double a1p = GetA1(x1p, x2p, h1, h2, [&](double x1, double x2) { return K(x1, x2, kai); });
				double a2p = GetA2(x1p, x2p, h1, h2, [&](double x1, double x2) { return K(x1, x2, kai); });

				int index = i * c + j;

				double h12 = h1 * h1;
				double h22 = h2 * h2;

				if (i > 1)
				{
					A[index][(i - 1) * c + j] = a1m / h12;
					A[index][i * c + j] -= a1m / h12;
				}
				if (i < c - 2)
				{
					A[index][(i + 1) * c + j] = a1p / h12;
					A[index][i * c + j] -= a1p / h12;
				}
				if (j > 1)
				{
					A[index][i * c + j - 1] = a2m / h22;
					A[index][i * c + j] -= a2m / h22;
				}
				if (j < c - 2)
				{
					A[index][i * c + j + 1] = a2p / h22;
					A[index][i * c + j] -= a2p / h22;
				}

				b[index] = F(x1m, x2m);
			}
		}

		for (int i = 0; i < c; ++i)
		{
			A[i][i] = 1.0;
			A[i + c * (c - 1)][i + c * (c - 1)] = 1.0;
			A[c * i][c * i] = 1.0;
			A[c - 1 + c * i][c - 1 + c * i] = 1.0;

			b[i] = u0;
			b[i + c * (c - 1)] = u0;
			b[c * i] = u0;
			b[c - 1 + c * i] = u0;
		}
	}

private: // Static Function
	static double K(double x1, double x2, double kai = 1.0)
	{
		double step = (x1 - 0.5) * (x1 - 0.5) + (x2 - 0.5) * (x2 - 0.5);
		return step > 0.125 ? 1.0 : kai;
	}

	static double F(double x1, double x2)
	{
		return 1.0;
	}

	static double GetA1(double x1, double x2, double h1, double h2, const std::function<double(double, double)>& k)
	{
		auto integrand = [&](double s2) -> double
		{
			return 1.0 / ((1.0 / h1) * IntegrandTrapezoidal([&](double s1) -> double
			{
				return 1.0 / k(s1, s2);
			}, x1 - h1, x1, 10));
		};

		return 1.0 / h2 * IntegrandTrapezoidal(integrand, x2 - 0.5 * h2, x2 + 0.5 * h2, 10);
	}

	static double GetA2(double x1, double x2, double h1, double h2, const std::function<double(double, double)>& k)
	{
		auto integrand = [&](double s1) -> double
		{
			return 1.0 / ((1.0 / h2) * IntegrandTrapezoidal([&](double s2) -> double
			{
				return 1.0 / k(s1, s2);
			}, x2 - h2, x2, 10));
		};

		return 1.0 / h1 * IntegrandTrapezoidal(integrand, x1 - 0.5 * h1, x1 + 0.5 * h1, 10);
	}

	static double IntegrandTrapezoidal(const std::function<double(double)>& func, double a, double b, double n)
	{
		double h = (b - a) / n;

		double sum = 0.0;

		for (int i = 1; i < n; ++i)
		{
			sum += func(a + i * h);
		}

		return h * (0.5 * (func(a) + func(b)) + sum);
	}

	static void IterateJacobi(const DMatX& A, const DVecX& b, DVecX& x, std::vector<double>& iterationCounts, std::vector<double>& residuals)
	{
		int count = 0;
		double r0 = Norm(A * x - b);
		double r = r0;

		int n = static_cast<int>(b.length());

		const DMatX B = Diagonal(A);
		DMatX B_inv = B;
		for (int i = 0; i < B.col_len(); ++i)
		{
			B_inv[i][i] = 1.0 / B[i][i];
		}

		auto rr = A * x - b;
		auto w = B_inv * rr;
		auto Aw = A * w;
		double tau = Dot(w, rr) / Dot(Aw, w);

		auto preX = x;
		x = B_inv * (B - tau * A) * x + tau * b;

		double alpha = 1.0;

		//DMatX S = DMatX(1.0, n, n) - B_inv * A;

		while (r > r0 * FLT_EPSILON)
		{
			x = ConjugateGradientNextX(A, x, b, B, B_inv, preX, rr, w, tau, alpha);
			//x = SeidelNextX(x, B_inv, S, b);

			r = Norm(A * x - b);

			iterationCounts.push_back(++count);
			residuals.push_back(r);
		}
	}

	static DVecX ConjugateGradientNextX(const DMatX& A, const DVecX& x, const DVecX& b, 
		const DMatX& B, const DMatX& B_inv, DVecX& prevX, DVecX& preR, DVecX& preW, double& preTau, double& preAlpha)
	{
		auto r = A * x - b;
		auto w = B_inv * r;
		auto Aw = A * w;
		double tau = Dot(w, r) / Dot(Aw, w);
		double alpha = 1.0 / (1.0 - (tau / preTau) * (Dot(w, r) / Dot(preW, preR)) / preAlpha);

		auto newX = B_inv * (alpha * (B - tau * A) * x + (1.0 - alpha) * B * prevX + alpha * tau * b);

		prevX = x;
		preTau = tau;
		preAlpha = alpha;
		preR = r;
		preW = w;

		return newX;
	}

	static DVecX SeidelNextX(const DVecX& x, const DMatX& B_inv, const DMatX& S, const DVecX& F)
	{
		return S * x + B_inv * F;
	}

private: // Data Field
	DVecX mX1;
	DVecX mX2;
	DVecX mY;
	DVecX mX1s[3];
	DVecX mX2s[3];
	DVecX mYs[3];
	std::vector<double> mIterationCounts;
	std::vector<double> mResiduals;

private: // State Field
	bool mComplete = true;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H10_1_P05", createFunc, castFunc, "Project")
			.RegisterField("n", &H10_1_P05::n, "int", "Range(10,500)")
			.RegisterField("kai", &H10_1_P05::kai, "double", "Range(0.01,100.0)");
	}
};  RCLASS_END(H10_1_P05)
