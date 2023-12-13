#include "Include/PlotGUI.h"

#include <random>

#include <vector>

#define MAX_ITERATION 10000

using namespace PlotGUI;
using namespace Math;

RCLASS H03_2_P05 : public Script
{
public:
	int n = 10;

private:
	struct Para
	{
		int n;
	};

public:
	void Start() override
	{
		mProfile = false;
	}

	void Update() override
	{
		if (mCalculate)
		{
			InitMatrixValue();

			double minEigenvalue1;
			DVecX minEigenvector1;
			{
				Profile("FindMinEigenPowerIteration");

				FindMinEigenPowerIteration(minEigenvalue1, minEigenvector1);
			}

			Console::Log("=======================================");
			Console::Log("When n = %d", n);
			Console::Log("=======================================");
			Console::Log("Use Power Iteration Method: ");
			Console::Log("    Min Eigen Value = %f", minEigenvalue1);
			if (n < 6)
			{
				std::string xString;
				xString << minEigenvector1;
				Console::Log("    Min Eigen Vector = %s", xString.c_str());
			}
			else
			{
				Console::Log("    Min Eigen Vector = [%f, %f, %f, %f, %f, ...]",
					minEigenvector1[0], minEigenvector1[1], minEigenvector1[2], minEigenvector1[3], minEigenvector1[4]);
			}
			Console::Log("Calculate time use: %f (ms)", Profiler::GetProfileInfo("FindMinEigenPowerIteration").deltaTime);
			Console::Log("=======================================");
			Console::Log("\n");
		}

		if (mProfile)
		{
			if (mCurrPara.n >= n)
			{
				mProfile = false;
				Console::Log("Finish!");
				return;
			}

			InitMatrixValue(mCurrPara);

			double minEigenvalue1;
			DVecX minEigenvector1;
			{
				Profile("FindMinEigenPowerIteration");

				FindMinEigenPowerIteration(mCurrPara, minEigenvalue1, minEigenvector1);
			}

			int index = mCurrPara.n;
			times[index] = Profiler::GetProfileInfo("FindMinEigenPowerIteration").deltaTime;
			Console::Log("Calculating n = %d || Time = %f (ms)", mCurrPara.n, times[index]);

			mCurrPara.n++;
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 03: practice05 \n\
Write a program to transform a symmetric real matrix A to a symmetric tridiagonal matrix PAP using the orthogonal Householder transform (P^{-1} = P). \
Using it, transform the matrix A, the elements of which are \n\
		a_ij = min(i, j), i = 1.2, ..., n, j = 1.2, ..., n \n\
for different n. \n\
\n\
Draw a graph of the execution time of the computational process depending on the size of the matrix.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H03_2_P05");

		ImGui::Spacing();

		mCalculate = ImGui::Button("Calculate!", ImVec2(-1.0f, 0.0f));

		if (ImGui::Button("Count iterations of different para", ImVec2(-1.0f, 0.0f)))
		{
			mProfile = !mProfile;
			if (mProfile)
			{
				mCurrPara.n = 10;

				x.assign(n, 0.0);
				for (int i = 0; i < n; ++i)
				{
					x[i] = i;
				}

				times.resize(n);
			}
		}
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;

		Plot::Plot1("Calculate Eigen Time", "dimensioni", "Time (ms)",
			x.data(), times.data(), x.size(), "Power Iteration", desc);
	}

private:
	void InitMatrixValue()
	{
		Para para;
		para.n = n;

		InitMatrixValue(para);
	}

	void InitMatrixValue(Para para)
	{
		mA = DMatX(0.0, para.n, para.n);

		for (int j = 0; j < para.n; ++j)
		{
			for (int i = 0; i < para.n; ++i)
			{
				mA[j][i] = Min(i, j);
			}
		}
	}

	void FindMinEigenHausholder(double& minEigenvalue, DVecX& minEigenvector)
	{
		Para para;
		para.n = n;

		FindMinEigenHausholder(para, minEigenvalue, minEigenvector);
	}

	void FindMinEigenHausholder(Para para, double& minEigenvalue, DVecX& minEigenvector)
	{
		DMatX Q, R;

		for (int i = 0; i < MAX_ITERATION; i++)
		{
			QRDecompose_HouseholderReflection(mA, Q, R);

			DMatX D = R * Q;
			DMatX T = D - Diagonal(D);

			if (CalculateA_E(T) < FLT_EPSILON)
			{
				return;
			}
		}

		DMatX invA = Inverse(mA);

		minEigenvector = DVecX(para.n, 1.0);

		for (int i = 0; i < MAX_ITERATION; ++i)
		{
			DVecX x1 = invA * minEigenvector;

			double value = Dot(minEigenvector, minEigenvector) / Dot(x1, minEigenvector);

			if (Abs(value - minEigenvalue) < FLT_EPSILON)
			{
				minEigenvalue = value;
				minEigenvector = Normailzed(x1);
				return;
			}

			minEigenvalue = value;
			minEigenvector = x1;
		}

		minEigenvector = Normailzed(minEigenvector);
	}

	double CalculateA_E(const Math::DMatX& A)
	{
		double res = 0.0;
		for (int j = 0; j < A.row_len(); ++j)
		{
			for (int i = 0; i < A.col_len(); ++i)
			{
				res += Math::Pow(A[j][i], 2.0);
			}
		}

		return Math::Sqrt(res);
	}

private:
	DMatX mA;

private:
	bool mCalculate = false;
	bool mProfile = false;

	Para mCurrPara;

	std::vector<double> x;
	std::vector<double> times;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H03_2_P05", createFunc, castFunc, "Project")
			.RegisterField("n", &H03_2_P05::n, "int", "Range(10,100)");
	}
};  RCLASS_END(H03_2_P05)
