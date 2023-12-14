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

			DMatX P, P_inv;
			{
				Profile("TransformTridiagonalMatrixHouseholder");

				TransformTridiagonalMatrixHouseholder(P_inv, P);
			}

			std::string xString0, xString1;
			xString0 << P;
			xString1 << P_inv * mA * P;

			Console::Log("=======================================");
			Console::Log("When n = %d", n);
			Console::Log("=======================================");
			Console::Log("Use Householder Method: ");
			Console::Log("P = %s", xString0.c_str());
			Console::Log("D = %s", xString1.c_str());
			Console::Log("Calculate time use: %f (ms)", Profiler::GetProfileInfo("TransformTridiagonalMatrixHouseholder").deltaTime);
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

			DMatX P, P_inv;
			{
				Profile("TransformTridiagonalMatrixHouseholder");

				TransformTridiagonalMatrixHouseholder(mCurrPara, P_inv, P);
			}

			int index = mCurrPara.n;
			times[index] = Profiler::GetProfileInfo("TransformTridiagonalMatrixHouseholder").deltaTime;
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
				mCurrPara.n = 1;

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

		Plot::Plot1("Calculate Transform Matrix P Time", "dimensioni", "Time (ms)",
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
				mA[j][i] = Min(i, j) + 1;
			}
		}
	}

	void TransformTridiagonalMatrixHouseholder(DMatX& P_inv, DMatX& P)
	{
		Para para;
		para.n = n;

		TransformTridiagonalMatrixHouseholder(para, P_inv, P);
	}

	void TransformTridiagonalMatrixHouseholder(Para para, DMatX& P_inv, DMatX& P)
	{
		DMatX Q, R;

		for (int i = 0; i < MAX_ITERATION; i++)
		{
			QRDecompose_HouseholderReflection(mA, Q, R);

			DMatX D = R * Q;
			DMatX T = D - Diagonal(D) - Diagonal(D, -1) - Diagonal(D, 1);

			if (Norm(T) < Epsilon)
			{
				P_inv = Inverse(Q);
				P = Q;

				return;
			}

			mA = D;
		}

		P_inv = Inverse(Q);
		P = Q;
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
			.RegisterField("n", &H03_2_P05::n, "int", "Range(1,20)");
	}
};  RCLASS_END(H03_2_P05)
