#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;

RCLASS H02_1_P04 : public Script
{
public:
	int n = 10;
	float alpha = 0.5;

private:
	struct Para
	{
		int n;
		float alpha;

		int i;
		int j;
	};

public:
	void Start() override
	{
		counts.assign(10 * 11, 0.0);
		mCountIter = false;
	}

	void Update() override
	{
		if (mCalculate)
		{
			InitMatrixValue();

			int count = Iterate();

			Console::Log("=======================================");
			Console::Log("When n = %d and alpha = %f", n, alpha);
			if (n < 6)
			{
				std::string xString;
				xString << mX;
				Console::Log("X = %s", xString.c_str());
			}
			else
			{
				Console::Log("X = [%f, %f, %f, %f, %f, ...]", mX[0], mX[1], mX[2], mX[3], mX[4]);
			}
			Console::Log("Iterate count is %d", count);
			Console::Log("=======================================");
			Console::Log("\n");
		}

		//if (mCountIter)
		//{
		//	if (mCurrPara.alpha > 1.01f)
		//	{
		//		mCurrPara.n += 10;
		//		mCurrPara.alpha = 0.0f;
		//
		//		mCurrPara.i--;
		//		mCurrPara.j = 0;
		//	}
		//
		//	if (mCurrPara.n > 100)
		//	{
		//		mCountIter = false;
		//		Console::Log("Finish!");
		//		return;
		//	}
		//
		//	InitMatrixValue(mCurrPara);
		//
		//	int count = Iterate(mCurrPara);
		//	int index = mCurrPara.i * 11 + mCurrPara.j;
		//	counts[index] = count;
		//	Console::Log("Calculating n = %d, alpha = %f || Count = %d ...", mCurrPara.n, mCurrPara.alpha, count);
		//
		//	mCurrPara.alpha += 0.1f;
		//	mCurrPara.j++;
		//}

		if (mCountIter1)
		{
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 11; ++j)
				{
					Threads::Run([&](int ii, int jj)
						{
							Para para;
							para.alpha = 0.1f * jj;
							para.n = 10 + 10 * ii;

							Math::DMatX A;
							Math::DVecX F;
							Math::DVecX X;

							InitMatrixValue(para, A, F, X);

							int count = Iterate(para, A, F, X);
							int index = (9 - ii) * 11 + jj;
							counts[index] = count;
							Console::Log("Calculating n = %d, alpha = %f || Count = %d ...", para.n, para.alpha, count);
						}, i, j);
				}
			}
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped(
			"Work 02: practice04 \n\
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

		//if (ImGui::Button("Count iterations of different para", ImVec2(-1.0f, 0.0f)))
		//{
		//	mCountIter = !mCountIter;
		//	if (mCountIter)
		//	{
		//		mCurrPara.n = 10;
		//		mCurrPara.alpha = 0.0f;
		//		mCurrPara.i = 9;
		//		mCurrPara.j = 0;
		//		mCurrIndex = 0;
		//	}
		//}

		mCountIter1 = ImGui::Button("Count iterations of different para 1", ImVec2(-1.0f, 0.0f));
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.plotFlags |= ImPlotFlags_NoLegend;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;

		AxesLimits limits;
		limits.xMin = 0.0;
		limits.xMax = 1.0;
		limits.yMin = 10.0;
		limits.yMax = 100.0;

		Plot::PlotColorMap2D("Iterations Count", "alpha", "dimension", counts.data(), 10, 11, "Count", desc, limits);
	}

private:
	void InitMatrixValue()
	{
		Para para;
		para.n = n;
		para.alpha = alpha;

		InitMatrixValue(para);
	}

	void InitMatrixValue(Para para)
	{
		mA = Math::DMatX(0.0, para.n, para.n);

		for (int i = 0; i < para.n; i++)
		{
			mA[i][i] = 2.0;
		}

		for (int i = 0; i < para.n - 1; i++)
		{
			mA[i + 1][i] = -1.0 - para.alpha;
		}

		for (int i = 1; i < para.n; i++)
		{
			mA[i - 1][i] = -1.0 + para.alpha;
		}

		mF = Math::DVecX(para.n, 0.0);
		mF[0] = 1.0 - static_cast<double>(para.alpha);
		mF[para.n - 1] = 1.0 + static_cast<double>(para.alpha);

		mX = Math::DVecX(para.n, 0.0);
	}

	void InitMatrixValue(Para para, Math::DMatX& A, Math::DVecX& F, Math::DVecX& X)
	{
		A = Math::DMatX(0.0, para.n, para.n);

		for (int i = 0; i < para.n; i++)
		{
			A[i][i] = 2.0;
		}

		for (int i = 0; i < para.n - 1; i++)
		{
			A[i + 1][i] = -1.0 - para.alpha;
		}

		for (int i = 1; i < para.n; i++)
		{
			A[i - 1][i] = -1.0 + para.alpha;
		}

		F = Math::DVecX(para.n, 0.0);
		F[0] = 1.0 - static_cast<double>(para.alpha);
		F[para.n - 1] = 1.0 + static_cast<double>(para.alpha);

		X = Math::DVecX(para.n, 0.0);
	}

	int Iterate()
	{
		Para para;
		para.n = n;
		para.alpha = alpha;

		return Iterate(para);
	}

	int Iterate(Para para)
	{
		int count = 0;
		double r0 = Math::Norm(mA * mX - mF);
		double r = r0;

		Math::DMatX B(0.0, para.n, para.n);
		Math::DMatX E(1.0, para.n, para.n);

		for (int j = 0; j < para.n; j++)
		{
			for (int i = j; i < para.n; i++)
			{
				B[j][i] = mA[j][i];
			}
		}

		Math::DMatX B_inv = Math::Inverse(B);
		Math::DMatX S = E - B_inv * mA;

		while (r > r0 * FLT_EPSILON)
		{
			mX = SeidelNextX(mX, B_inv, S);
			r = Math::Norm(mA * mX - mF);
			count++;
		}

		return count;
	}

	int Iterate(Para para, Math::DMatX& A, Math::DVecX& F, Math::DVecX& X)
	{
		int count = 0;
		double r0 = Math::Norm(A * X - F);
		double r = r0;

		Math::DMatX B(0.0, para.n, para.n);
		Math::DMatX E(1.0, para.n, para.n);

		for (int j = 0; j < para.n; j++)
		{
			for (int i = j; i < para.n; i++)
			{
				B[j][i] = A[j][i];
			}
		}

		Math::DMatX B_inv = Math::Inverse(B);
		Math::DMatX S = E - B_inv * A;

		while (r > r0 * FLT_EPSILON)
		{
			X = SeidelNextX(X, B_inv, S, F);
			r = Math::Norm(A * X - F);
			count++;
		}

		return count;
	}

	Math::DVecX SeidelNextX(const Math::DVecX& x, const Math::DMatX& B_inv, const Math::DMatX& S)
	{
		return S * x + B_inv * mF;
	}

	Math::DVecX SeidelNextX(const Math::DVecX& x, const Math::DMatX& B_inv, const Math::DMatX& S, Math::DVecX& F)
	{
		return S * x + B_inv * F;
	}

private:
	Math::DMatX mA;
	Math::DVecX mF;
	Math::DVecX mX;

private:
	bool mCalculate = false;
	bool mCountIter = false;
	bool mCountIter1 = false;

	Para mCurrPara;
	int mCurrIndex = 0;
	std::vector<int> counts;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H02_1_P04", createFunc, castFunc, "Project")
			.RegisterField("n", &H02_1_P04::n, "int", "Range(10,100)")
			.RegisterField("alpha", &H02_1_P04::alpha, "float", "Range(0.0,1.0)");
	}
};  RCLASS_END(H02_1_P04)
