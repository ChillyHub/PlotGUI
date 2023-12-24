#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H09_1_P05 : public Script
{
public: // Inspector Field
	double l = 1.0;
	double miu1 = 0.0;
	double miu2 = 0.0;
	double h0 = 0.01;

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
		if (mComplete)
		{
			mPlotCoefficients[0] = mCoefficients[0];
			mPlotCoefficients[1] = mCoefficients[1];
			mPlotCoefficients[2] = mCoefficients[2];

			mComplete = false;

			Threads::Run([&]()
			{
				const double divs[3] = { 1.0, 2.0, 4.0 };

				for (int i = 0; i < 3; ++i)
				{
					double h = h0 / divs[i];

					DVecX x, y;
					SolveDifferenceMethod(0.0001, l, miu1, miu2, h, x, y);

					mCoefficients[i] = GetNaturalCubicSplineCoefficients(x, y);
				}

				mComplete = true;
			});
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 09: practice05");
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

		Inspector::ShowRegisteredFields(this, "H09_1_P05");
	}

	void OnPlot() override
	{
		PlotDescriptor desc1;
		desc1.plotFlags |= ImPlotFlags_Equal;

		Plot::Plot3G<double>("Solve Differential Equations -- Difference Method", "", "", 400,
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mPlotCoefficients[0]);
			}, "u(x) -- h = h0",
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mPlotCoefficients[1]);
			}, "u(x) -- h = h0 / 2",
			[&](double x) -> double
			{
				return CubicSplineFunction(x, mPlotCoefficients[2]);
			}, "u(x) -- h = h0 / 4",
			mPlotCoefficients[0].x.data(), mPlotCoefficients[0].y.data(), mPlotCoefficients[0].count, desc1);
	}

private: // Class Function

private: // Static Function
	static double K(double x)
	{
		return (0.0 < x && x < 0.5) ? 1.0 : (0.5 < x && x < 1.0) ? 100.0 : NAN;
	}

	static double F(double x)
	{
		return 0;
	}

	static double GetA(int i, double a, double h, const std::function<double(double)>& k)
	{
		return 1.0 / ((1.0 / h) * IntegrandTrapezoidal([&](double x) -> double
		{
			return 1.0 / k(x);
		}, a + (i - 1) * h, a + i * h, 10));
	}

	static double GetPhi(int i, double a, double h, const std::function<double(double)>& k, const std::function<double(double)>& f)
	{
		auto integrand = [&](double x) -> double
		{
			return f(x);
		};

		auto integrand1 = [&](double x) -> double
		{
			return 1.0 / k(x) * IntegrandTrapezoidal(integrand, a + i * h, x, 10);
		};

		auto integrand2 = [&](double x) -> double
		{
			return 1.0 / k(x) * IntegrandTrapezoidal(integrand, x, a + i * h, 10);
		};

		double t1 = GetA(i + 1, a, h, k) * IntegrandTrapezoidal(integrand1, a + i * h, a + (i + 1) * h, 10);
		double t2 = GetA(i, a, h, k) * IntegrandTrapezoidal(integrand1, a + (i - 1) * h, a + i * h, 10);

		return (t1 + t2) / (h * h);
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

	static void SolveDifferenceMethod(double a, double b, double y0, double yn, double h, DVecX& x, DVecX& y)
	{
		int n = static_cast<int>(Floor((b - a) / h));

		DVecX alpha, gama, beta, phi;
		CoefficientsTrapezoidal(a, b, h, n - 1, y0, yn, alpha, gama, beta, phi);

		x = DVecX(n + 2);
		for (int i = 0; i <= n; ++i)
		{
			x[i] = a + i * h;
		}
		x[n + 1] = b;

		y = SolveThomas(alpha, gama, beta, phi);

		y[0] = y0;
		y[n + 1] = yn;
	}

	static void CoefficientsTrapezoidal(double a, double b, double h, int n, double y0, double yn, DVecX& alpha, DVecX& gama, DVecX& beta, DVecX& phi)
	{
		alpha = DVecX(n);
		beta = DVecX(n);
		gama = DVecX(n + 1);
		phi = DVecX(n + 1);

		for (int i = 1; i < n; ++i) 
		{
			double a_i = GetA(i + 1, a, h, K);
			double a_i1 = GetA(i + 2, a, h, K);

			alpha[i - 1] = -a_i / (h * h);
			beta[i] = -a_i1 / (h * h);
			gama[i] = (a_i + a_i1) / (h * h);
			phi[i] = GetPhi(i, a, h, K, F);
		}

		double a_0 = GetA(1, a, h, K);
		double a_1 = GetA(2, a, h, K);
		double a_n_1 = GetA(n, a, h, K);
		double a_n = GetA(n + 1, a, h, K);

		beta[0] = -a_1 / (h * h);
		gama[0] = (a_0 + a_1) / (h * h);
		alpha[n - 1] = -a_n / (h * h);
		gama[n] = (a_n_1 + a_n) / (h * h);

		phi[0] = y0;
		phi[n] = yn;
	}

	static DVecX SolveThomas(const DVecX& alpha, const DVecX& gama, const DVecX& beta, const DVecX& phi)
	{
		int n = static_cast<int>(alpha.length());

		DVecX y(n + 3);
		DVecX p(n + 1);
		DVecX q(n + 1);

		p[0] = -beta[0] / gama[0];
		q[0] = phi[0] / gama[0];

		for (int i = 1; i < n; ++i)
		{
			p[i] = -beta[i] / (gama[i] + alpha[i - 1] * p[i - 1]);
			q[i] = (phi[i] - alpha[i - 1] * q[i - 1]) / (gama[i] + alpha[i - 1] * p[i - 1]);
		}

		y[n + 1] = (phi[n] - alpha[n - 1] * q[n - 1]) / (gama[n] + alpha[n - 1] * p[n - 1]);

		for (int i = n - 1; i >= 0; --i)
		{
			y[i + 1] = p[i] * y[i + 1] + q[i];
		}

		return y;
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
	CubicSplineCoefficients mCoefficients[3];
	CubicSplineCoefficients mPlotCoefficients[3];

private: // State Field
	bool mComplete = true;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H09_1_P05", createFunc, castFunc, "Project")
			.RegisterField("l", &H09_1_P05::l, "double")
			.RegisterField("miu1", &H09_1_P05::miu1, "double")
			.RegisterField("miu2", &H09_1_P05::miu2, "double")
			.RegisterField("h0", &H09_1_P05::h0, "double", "Range(0.0001,0.1)");
	}
};  RCLASS_END(H09_1_P05)
