#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H10_1_P05 : public Script
{
public: // Inspector Field
	int n = 10;
	double kai = 1.0;

private: // Custom Struct

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		if (mComplete)
		{
			mX1c = mX1;
			mX2c = mX2;
			mYc = mY;

			mComplete = false;

			Threads::Run([&]()
				{
					std::vector<double> _a, _b;

					SolveDifferenceMethod(0.0, 1.0, 0.0, 1.0, 0.0, n, kai, mX1, mX2, mY, _a, _b);

					mComplete = true;
				});
		}

		if (mCalculate)
		{
			for (int i = 0; i < 3; ++i)
			{
				mIterationCounts[i].clear();
				mResiduals[i].clear();

				Threads::Run([&](int ii)
					{
						double kais[3] = { 0.01, 1.0, 100.0 };

						SolveDifferenceMethod(0.0, 1.0, 0.0, 1.0, 0.0, n, kais[ii], mX1s[ii], mX2s[ii], mYs[ii], mIterationCounts[ii], mResiduals[ii]);

						Console::Log("Finish Calculate in (n = %d, kai = %f)", n, kais[ii]);

						mCompletes[ii] = true;
					}, i);
			}
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 10: practice05");
		ImGui::TextWrapped("Write a program for numerical solution on a grid of the problem that is uniform in each direction \n\
in a rectangle Omega = [0, 1] x [0, 1]. Use the Jacobi iterative method (B = D) and the choice of iterative parameters using the conjugate gradient method. \
Using this program, solve the unit square boundary value problem f(x) = 1 and \n\
at Kai = 10^-2, 1, 10^2 \n\
- Draw a numerical solution \n\
- Investigate the convergence of the method");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H10_1_P05");

		mCalculate = ImGui::Button("Calculate in different Kai", { -1.0f, 0.0f });
		mShowCalculate |= mCalculate;

		mShowResiduals |= ImGui::Button("Show Residuals in different Kai", { -1.0f, 0.0f });
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

		int c = static_cast<int>(Sqrt(mYc.length()));

		Plot::PlotColorMap2DL("Solve Differential Equations -- Difference, Jacobi, ConjugateGradient Method", "x1", "x2",
			mYc.data(), c, c, 400, 400, "u(x1, x2)", desc1, limits);

		if (mShowCalculate)
		{
			double kais[3] = { 0.01, 1.0, 100.0 };

			for (int k = 0; k < 3; ++k)
			{
				std::string title = "Solve Differential Equations -- Kai = " + std::to_string(kais[k]);

				int cc = static_cast<int>(Sqrt(mYs[k].length()));

				Plot::PlotColorMap2DL(title, "x1", "x2",
					mYs[k].data(), cc, cc, 400, 400, "u(x1, x2)", desc1, limits);
			}
		}

		if (mShowResiduals)
		{
			PlotDescriptor desc2;
			desc2.axisFlags |= ImPlotAxisFlags_AutoFit;

			std::string kais[3] = { "Kai = 0.01", "Kai = 1", "Kai = 100" };

			Plot::Plot3("Residuals", "Iterations Count", "Residuals",
				mIterationCounts[0].data(), mResiduals[0].data(), mIterationCounts[0].size(), kais[0],
				mIterationCounts[1].data(), mResiduals[1].data(), mIterationCounts[1].size(), kais[1],
				mIterationCounts[2].data(), mResiduals[2].data(), mIterationCounts[2].size(), kais[2], desc2);
		}
	}

private: // Class Function
	void SolveDifferenceMethod(double a1, double b1, double a2, double b2, double u0, int n, double kai, 
		DVecX& x1, DVecX& x2, DVecX& y, std::vector<double>& iterationCounts, std::vector<double>& residuals)
	{
		DMatX A;
		DVecX b;
		CoefficientsTrapezoidal(a1, b1, a2, b2, n, u0, kai, A, b);

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

				if (i > 0)
				{
					if (i > 1)
					{
						A[index][(i - 1) * c + j] = a1m / h12;
					}
					
					A[index][i * c + j] -= a1m / h12;
				}
				if (i < c - 1)
				{
					if (i < c - 2)
					{
						A[index][(i + 1) * c + j] = a1p / h12;
					}
					
					A[index][i * c + j] -= a1p / h12;
				}
				if (j > 0)
				{
					if (j > 1)
					{
						A[index][i * c + j - 1] = a2m / h22;
					}
					
					A[index][i * c + j] -= a2m / h22;
				}
				if (j < c - 1)
				{
					if (j < c - 2)
					{
						A[index][i * c + j + 1] = a2p / h22;
					}
					
					A[index][i * c + j] -= a2p / h22;
				}

				b[index] = -F(x1m, x2m);
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

		DMatX S = DMatX(1.0, n, n) - B_inv * A;

		while (r > r0 * FLT_EPSILON)
		{
			//x = ConjugateGradientNextX(A, x, b, B, B_inv, preX, rr, w, tau, alpha);
			x = SeidelNextX(x, B_inv, S, b);

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
	DVecX mX1c;
	DVecX mX2c;
	DVecX mYc;
	DVecX mX1s[3];
	DVecX mX2s[3];
	DVecX mYs[3];
	
	std::vector<double> mIterationCounts[3];
	std::vector<double> mResiduals[3];

private: // State Field
	bool mComplete = true;
	bool mCompletes[3] = { true, true, true };
	bool mCalculate = false;
	bool mShowCalculate = false;
	bool mShowResiduals = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H10_1_P05", createFunc, castFunc, "Project")
			.RegisterField("n", &H10_1_P05::n, "int", "Range(10,30)")
			.RegisterField("kai", &H10_1_P05::kai, "double", "Range(0.01,100.0)");
	}
};  RCLASS_END(H10_1_P05)
