#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;

RCLASS H01_1_P04 : public Script
{
public:
	int matrixDim = 4;

public:
	void Start() override
	{
		mCurrDim = matrixDim;
		dims.assign(mCurrDim, 0.0);
		times.assign(mCurrDim, 0.0);
	}

	void Update() override
	{
		if (mCurrDim == matrixDim)
		{
			mProfile = false;
		}

		if (!mCalculate && !mProfile)
		{
			return;
		}

		if (!mProfile)
		{
			mCurrDim = matrixDim;
		}
		else
		{
			mCurrDim = Math::Min(mCurrDim + 1, matrixDim);
		}

		if (times.size() <= mCurrDim)
		{
			dims.resize(mCurrDim + 1);
			times.resize(mCurrDim + 1);
		}

		// Init A
		Math::DMatX A(mCurrDim, mCurrDim);

		std::default_random_engine dre;
		std::uniform_real_distribution<> distr(-100.0, 100.0);

		for (int i = 0; i < mCurrDim; ++i)
		{
			for (int j = 0; j < mCurrDim; ++j)
			{
				A[i][j] = distr(dre);
			}
		}

		double A_1, A_E, A_u;
		{
			Profile("Calculate Matrix Norm");

			// Calculate ||A||_1
			A_1 = CalculateA_1(A);

			// Calculate ||A||_E
			A_E = CalculateA_E(A);

			// Calculate ||A||_unlimited
			A_u = CalculateA_unlimited(A);
		}

		double sqrtN = Math::Sqrt(static_cast<double>(mCurrDim));

		double a1 = A_1 / sqrtN;
		double b1 = A_E;
		double c1 = A_1 * sqrtN;

		double au = A_u / sqrtN;
		double bu = A_E;
		double cu = A_u * sqrtN;

		bool inequality1 = a1 <= b1 && b1 <= c1;
		bool inequality2 = au <= bu && bu <= cu;

		std::string s1 = inequality1 ? "True" : "False";
		std::string s2 = inequality2 ? "True" : "False";

		double deltaTime = Profiler::GetProfileInfo("Calculate Matrix Norm").deltaTime;

		dims[mCurrDim] = mCurrDim;
		times[mCurrDim] = deltaTime;

		if (mCurrDim < matrixDim)
		{
			Console::Log("Calculating matrix of dimensions %d", mCurrDim);
			return;
		}

		Console::Log("=======================================");
		Console::Log("If dimensions N = %d:", mCurrDim);
		Console::Log("Sqrt(N)         = %f", sqrtN);
		Console::Log("||A||_1         = %f", A_1);
		Console::Log("||A||_E         = %f", A_E);
		Console::Log("||A||_unlimited = %f", A_u);
		Console::Log("\n");
		Console::Log("||A||_1 / sqrtN = %f", A_1 / sqrtN);
		Console::Log("||A||_E         = %f", A_E);
		Console::Log("||A||_1 * sqrtN = %f", A_1 * sqrtN);
		Console::Log("\n");
		Console::Log("||A||_u / sqrtN = %f", A_u / sqrtN);
		Console::Log("||A||_E         = %f", A_E);
		Console::Log("||A||_u * sqrtN = %f", A_u * sqrtN);
		Console::Log("\n");
		Console::Log("Inequality 1 is %s", s1.c_str());
		Console::Log("Inequality 2 is %s", s2.c_str());
		Console::Log("=======================================");
		Console::Log("When dimensions is %d", mCurrDim);
		Console::Log("Calculate Time: %f (ms)", deltaTime);
		Console::Log("=======================================");
		Console::Log("\n");
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped(
			"Work 01: practice04 \n\
		Write a program to calculate the norms of real vectors (||x||_p, p = 1,2, unlimited) and the norms of real matrices...\n\n\
		Based on experiments with random matrices for different n, verify the inequalities...\n\n\
		for the Euclidean(spherical) norm(Frobenius norm) ||A||_E.\n\
		Construct a graph of the dependence of the program execution time on the size of the matrix of the system of equations.");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H01_1_P04");

		ImGui::Spacing();

		mCalculate = ImGui::Button("Calculate!", ImVec2(-1.0f, 0.0f));
		if (mCalculate)
		{
			mCurrDim = matrixDim;
		}

		if (ImGui::Button("Profile Time", ImVec2(-1.0f, 0.0f)))
		{
			mProfile = true;
			mCurrDim = 1;
		}
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;
		desc.lineFlags |= ImPlotLineFlags_SkipNaN;

		Plot::Plot1("Calculate Time of different dimensions", "dim", "time(ms)", dims.data(), times.data(), times.size(), "Time", desc);
	}

private:
	double CalculateA_1(const Math::DMatX& A)
	{
		double res = 0.0;
		for (int j = 0; j < A.row_len(); ++j)
		{
			double sum = 0.0;
			for (int i = 0; i < A.col_len(); ++i)
			{
				sum += Math::Abs(A[j][i]);
			}

			res = Math::Max(res, sum);
		}

		return res;
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

	double CalculateA_unlimited(const Math::DMatX& A)
	{
		double res = 0.0;
		for (int i = 0; i < A.col_len(); ++i)
		{
			double sum = 0.0;
			for (int j = 0; j < A.row_len(); ++j)
			{
				sum += Math::Abs(A[j][i]);
			}

			res = Math::Max(res, sum);
		}

		return res;
	}

private:
	int mCurrDim;
	bool mCalculate = false;
	bool mProfile = false;
	std::vector<double> dims;
	std::vector<double> times;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H01_1_P04", createFunc, castFunc, "Project")
			.RegisterField("matrixDim", &H01_1_P04::matrixDim, "int");
	}
};  RCLASS_END(H01_1_P04)
