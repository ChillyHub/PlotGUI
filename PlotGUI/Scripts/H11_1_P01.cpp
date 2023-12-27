#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;
using namespace Math;

RCLASS H11_1_P01 : public Script
{
public: // Inspector Field
	int nx = 10;
	double l = 1.0;
	double t = 1.0;
	double sigma = 0.5;

private: // Custom Struct
	struct DifferenceArgs
	{
		double hx;
		double ht;
		int npx;
		int npt;
	};

	struct Solution
	{
		DVecX xt;
		int npx;
		int npt;
	};

public: // Override Func
	void Start() override
	{

	}

	void Update() override
	{
		if (mComplete)
		{
			mPlotSolution = mSolution;

			mComplete = false;

			Threads::Run([&]()
			{
				DifferenceArgs args = GetDifferenceArgs(0.0, t, 0.0, l, nx);

				DVecX u0(args.npx - 2, 0.0);
				for (int i = 1; i < args.npx - 1; ++i)
				{
					u0[i - 1] = U(l / nx * i, 0.0);
				}

				DVecX ult0(args.npx - 2, 0.0);
				for (int i = 1; i < args.npx - 1; ++i)
				{
					ult0[i - 1] = Ult(l / nx * i, 0.0);
				}

				DVecX ux0(args.npt, 0.0);
				for (int i = 0; i < args.npt; ++i)
				{
					ux0[i] = U0t();
				}

				DVecX uxl(args.npt, 0.0);
				for (int i = 0; i < args.npt; ++i)
				{
					uxl[i] = Uxt();
				}

				DMatX xt = SolveDifferenceMethod(args, sigma, u0, ult0, ux0, uxl);

				DVecX sol(args.npx * args.npt, 0.0);
				for (int i = 0; i < args.npx; ++i)
				{
					for (int j = 0; j < args.npt; ++j)
					{
						int index = i * args.npt + j;

						sol[index] = xt[i][j];
					}
				}

				mSolution.xt = sol;
				mSolution.npx = args.npx;
				mSolution.npt = args.npt;

				mComplete = true;
			});
		}

		if (mCalculate)
		{
			for (int i = 0; i < 3; ++i)
			{
				//Threads::Run([&](int ii)
				//	{
				//		double kais[3] = { 0.01, 1.0, 100.0 };
				//
				//		SolveDifferenceMethod(0.0, 1.0, 0.0, 1.0, 0.0, n, kais[ii], mX1s[ii], mX2s[ii], mYs[ii], mIterationCounts[ii], mResiduals[ii]);
				//
				//		Console::Log("Finish Calculate in (n = %d, kai = %f)", n, kais[ii]);
				//
				//		mCompletes[ii] = true;
				//	}, i);
			}
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 11: practice01");
		ImGui::TextWrapped("");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H11_1_P01");

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
		limits.xMax = t;
		limits.yMin = 0.0;
		limits.yMax = l;

		Plot::PlotColorMap2DL("Solve Differential Equations -- Stability", "t", "x", "u(x, t)", 
			mPlotSolution.xt.data(), mPlotSolution.npx, mPlotSolution.npt, mPlotSolution.npx * 10.0, mPlotSolution.npt * 10.0, "u(x, t)", desc1, limits);

		//if (mShowCalculate)
		//{
		//	double kais[3] = { 0.01, 1.0, 100.0 };
		//
		//	for (int k = 0; k < 3; ++k)
		//	{
		//		std::string title = "Solve Differential Equations -- Kai = " + std::to_string(kais[k]);
		//
		//		int cc = static_cast<int>(Sqrt(mYs[k].length()));
		//
		//		Plot::PlotColorMap2DL(title, "x1", "x2",
		//			mYs[k].data(), cc, cc, 400, 400, "u(x1, x2)", desc1, limits);
		//	}
		//}
	}

private: // Class Function

private: // Static Function
	static double U(double x, double t)
	{
		return Exp(-PI * PI * t) * Sin(PI * x) + Exp(-16.0 * PI * PI * t) * Sin(4.0 * PI * x);
	}

	static double Ult(double x, double t)
	{
		return -PI * PI * Exp(-PI * PI * t) * Sin(PI * x) + -16.0 * PI * PI * Exp(-16.0 * PI * PI * t) * Sin(4.0 * PI * x);
	}

	static double U0t()
	{
		return 0.0;
	}

	static double Uxt()
	{
		return 0.0;
	}

	DifferenceArgs GetDifferenceArgs(double at, double bt, double ax, double bx, int nx)
	{
		double hx = (bx - ax) / nx;
		double ht = sigma * hx;

		int npx = nx + 1;
		int npt = static_cast<int>((bt - at) / ht) + 1;

		return { hx, ht, npx, npt };
	}

	DMatX SolveDifferenceMethod(DifferenceArgs args, double sigma, const DVecX& u0, const DVecX& ult0, const DVecX& ux0, const DVecX& uxl)
	{
		double hx = args.hx;
		double ht = args.ht;
		int npx = args.npx;
		int npt = args.npt;

		DMatX A = GetDifferenceMatrix(npx - 2, sigma);

		DMatX xt(0.0, npx - 2, npt);
		xt[0] = u0;
		xt[1] = 0.5 * A * u0 + ht * ult0;

		for (int i = 2; i < npt; ++i)
		{
			xt[i] = A * xt[i - 1] - xt[i - 2];
		}

		DMatX res(0.0, npx, npt);
		res[0][0] = 0.5 * sigma * sigma * ux0[0];
		res[0][npx - 1] = 0.5 * sigma * sigma * uxl[0];
		for (int i = 0; i < npx - 2; ++i)
		{
			res[0][i + 1] = xt[0][i];
		}
		for (int i = 1; i < npt; ++i)
		{
			res[i][0] = sigma * sigma * ux0[i];
			for (int j = 0; j < npx - 2; ++j)
			{
				res[i][j + 1] = xt[i][j];
			}
			res[i][npx - 1] = sigma * sigma * uxl[i];
		}

		return Transpose(res);
	}

	static DMatX GetDifferenceMatrix(int n, double sigma)
	{
		DMatX A(0.0, n, n);
		double sigma2 = sigma * sigma;

		for (int i = 0; i < n; ++i)
		{
			A[i][i] = 2.0 - 2.0 * sigma2;
		}

		for (int i = 0; i < n - 1; ++i)
		{
			A[i][i + 1] = sigma2;
		}

		for (int i = 1; i < n; ++i)
		{
			A[i][i - 1] = sigma2;
		}

		return A;
	}

private: // Data Field
	Solution mSolution;
	Solution mPlotSolution;

private: // State Field
	bool mComplete = true;
	bool mCompletes[3] = { true, true, true };
	bool mCalculate = false;
	bool mShowCalculate = false;
	bool mShowResiduals = false;

public: // Register Function
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H11_1_P01", createFunc, castFunc, "Project")
			.RegisterField("nx", &H11_1_P01::nx, "int", "Range(10,30)")
			.RegisterField("l", &H11_1_P01::l, "double", "Range(0.01,5.0)")
			.RegisterField("t", &H11_1_P01::t, "double", "Range(1.0,5.0)")
			.RegisterField("sigma", &H11_1_P01::sigma, "double", "Range(0.05,1.0)");
	}
};  RCLASS_END(H11_1_P01)
