#include "Include/PlotGUI.h"

#include <random>

#include <vector>

#define MAX_ITERATION 10000

using namespace PlotGUI;
using namespace Math;

RCLASS H03_1_P03 : public Script
{
public:
	int n = 10;

private:
	struct Para
	{
		int n;

		// index
		int i;
	};

public:
	void Start() override
	{
		counts.assign(10 * 11, 0.0);
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


			double minEigenvalue2;
			DVecX minEigenvector2;
			{
				Profile("FindMinEigenJacobi");
				
				FindMinEigenJacobi(minEigenvalue2, minEigenvector2);
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
			Console::Log("Use Jacobi Method: ");
			Console::Log("    Min Eigen Value = %f", minEigenvalue2);
			if (n < 6)
			{
				std::string xString;
				xString << minEigenvector2;
				Console::Log("    Min Eigen Vector = %s", xString.c_str());
			}
			else
			{
				Console::Log("    Min Eigen Vector = [%f, %f, %f, %f, %f, ...]",
					minEigenvector2[0], minEigenvector2[1], minEigenvector2[2], minEigenvector2[3], minEigenvector2[4]);
			}
			Console::Log("Calculate time use: %f (ms)", Profiler::GetProfileInfo("FindMinEigenJacobi").deltaTime);
			Console::Log("=======================================");
			Console::Log("\n");
		}

		if (mProfile)
		{
			//if (mCurrPara.n > 100)
			//{
			//	mProfile = false;
			//	Console::Log("Finish!");
			//	return;
			//}
			//
			//InitMatrixValue(mCurrPara);
			//
			//int count = Iterate(mCurrPara);
			//int index = mCurrPara.i * 11 + mCurrPara.j;
			//counts[index] = count;
			//Console::Log("Calculating n = %d, alpha = %f || Count = %d ...", mCurrPara.n, mCurrPara.alpha, count);
			//
			//mCurrPara.alpha += 0.1f;
			//mCurrPara.j++;
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped("Work 03: practice03 \n\
Write a program to find the minimum absolute eigenvalue and the corresponding eigenvector of a symmetric tridiagonal matrix. \
Using it, find the first minimum modulo eigenvalue of the n x n matrix for which \n\
		a_ii = 2, a_ii-1 = aii+1 = -1 \n\
for different n. Compare the numerical solution with the exact one. \n\
\n\
Draw a graph of the execution time of the computational process depending on the size of the matrix.");

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

		Inspector::ShowRegisteredFields(this, "H03_1_P03");

		ImGui::Spacing();

		mCalculate = ImGui::Button("Calculate!", ImVec2(-1.0f, 0.0f));

		if (ImGui::Button("Count iterations of different para", ImVec2(-1.0f, 0.0f)))
		{
			mProfile = !mProfile;
			if (mProfile)
			{
				mCurrPara.n = 10;
				mCurrPara.i = 9;
				mCurrIndex = 0;
			}
		}
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

		InitMatrixValue(para);
	}

	void InitMatrixValue(Para para)
	{
		mA = DMatX(2.0, para.n, para.n);

		for (int i = 1; i < para.n; ++i)
		{
			mA[i - 1][i] = -1.0;
		}

		for (int i = 0; i < para.n - 1; ++i)
		{
			mA[i + 1][i] = -1.0;
		}
	}

	void FindMinEigenPowerIteration(double& minEigenvalue, DVecX& minEigenvector)
	{
		Para para;
		para.n = n;

		FindMinEigenPowerIteration(para, minEigenvalue, minEigenvector);
	}

	void FindMinEigenPowerIteration(Para para, double& minEigenvalue, DVecX& minEigenvector)
	{
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

	void FindMinEigenJacobi(double& minEigenvalue, DVecX& minEigenvector)
	{
		Para para;
		para.n = n;

		FindMinEigenJacobi(para, minEigenvalue, minEigenvector);
	}

	void FindMinEigenJacobi(Para para, double& minEigenvalue, DVecX& minEigenvector)
	{
		DMatX Q, R;
		QRDecompose_HouseholderReflection(mA, Q, R);

		std::cout << Q << std::endl;
		std::cout << R << std::endl;
	}

private:
	DMatX mA;

private:
	bool mCalculate = false;
	bool mProfile = false;

	Para mCurrPara;
	int mCurrIndex = 0;
	std::vector<int> counts;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H03_1_P03", createFunc, castFunc, "Project")
			.RegisterField("n", &H03_1_P03::n, "int", "Range(10,100)");
	}
};  RCLASS_END(H03_1_P03)
