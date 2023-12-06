#include "Include/PlotGUI.h"

#include <random>

#include <vector>

using namespace PlotGUI;

RCLASS H02_2_P05 : public Script
{
public:
	int n = 10;

private:
	struct Para
	{
		int n;
		int i;
	};

public:
	void Start() override
	{
		xs = std::vector<int>{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
		counts1.assign(10, 0.0);
		counts2.assign(10, 0.0);
		mCountIter = false;
	}

	void Update() override
	{
		if (mCalculate)
		{
			InitMatrixValue();

			int count1 = Iterate1();

			Console::Log("=======================================");
			Console::Log("When n = %d, use Minimal Corrections Method", n);
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
			Console::Log("Iterate count is %d", count1);

			InitMatrixValue();

			int count2 = Iterate2();

			Console::Log("=======================================");
			Console::Log("When n = %d, use Conjugate Gradient Method", n);
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
			Console::Log("Iterate count is %d", count2);
			Console::Log("=======================================");
			Console::Log("\n");
		}

		if (mCountIter)
		{
			if (mCurrPara.n > 100)
			{
				mCountIter = false;
				Console::Log("Finish!");
				return;
			}

			InitMatrixValue(mCurrPara);
			int count1 = Iterate1(mCurrPara);

			InitMatrixValue(mCurrPara);
			int count2 = Iterate2(mCurrPara);

			counts1[mCurrPara.i] = count1;
			counts2[mCurrPara.i] = count2;
			Console::Log("Calculating n = %d || Minimal Corrections Count = %d, Conjugate Gradient Count = %d ...", mCurrPara.n, count1, count2);

			mCurrPara.n += 10;
			mCurrPara.i++;
		}
	}

	void OnChange() override
	{

	}

	void OnInspector() override
	{
		ImGui::Spacing();

		ImGui::TextWrapped(
			"Work 02: practice05 \n\
		Write a program to solve a system of linear algebraic equations using the explicit (B = E) iterative method of minimal corrections \
		(iteration parameters are determined according to (23)). \
		Compare the convergence rate of this method with the convergence rate of the conjugate gradient method using the example problem");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();

		Inspector::ShowRegisteredFields(this, "H02_2_P05");

		ImGui::Spacing();

		mCalculate = ImGui::Button("Calculate!", ImVec2(-1.0f, 0.0f));

		if (ImGui::Button("Count iterations of different para", ImVec2(-1.0f, 0.0f)))
		{
			mCountIter = !mCountIter;
			if (mCountIter)
			{
				mCurrPara.n = 10;
				mCurrPara.i = 0;
			}
		}
	}

	void OnPlot() override
	{
		PlotDescriptor desc;
		desc.axisFlags |= ImPlotAxisFlags_AutoFit;

		Plot::Plot2("Iterations Count", "Dimension", "Count", 
			xs.data(), counts1.data(), xs.size(), "Minimal Corrections", 
			xs.data(), counts2.data(), xs.size(), "Conjugate Gradient", 
			desc);
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
		mA = Math::DMatX(0.0, para.n, para.n);
		mF = Math::DVecX(para.n, 0.0);
		mX = Math::DVecX(para.n, 0.0);

		for (int j = 0; j < para.n; ++j)
		{
			for (int i = 0; i < para.n; ++i)
			{
				mA[j][i] = 1.0 / (i + j + 1.0);
			}
		}

		for (int i = 0; i < para.n; ++i)
		{
			for (int j = 0; j < para.n; ++j)
			{
				mF[i] += mA[j][i];
			}
		}
	}

	int Iterate1()
	{
		Para para;
		para.n = n;

		return Iterate1(para);
	}

	int Iterate1(Para para)
	{
		int count = 0;
		double r0 = Math::Norm(mA * mX - mF);
		double r = r0;

		Math::DMatX B(1.0, para.n, para.n); // E

		const Math::DMatX& B_inv = B;

		while (r > r0 * FLT_EPSILON)
		{
			mX = MinimalCorrectionsNextX(B_inv);
			r = Math::Norm(mA * mX - mF);
			count++;
		}

		return count;
	}

	int Iterate2()
	{
		Para para;
		para.n = n;

		return Iterate2(para);
	}

	int Iterate2(Para para)
	{
		int count = 0;
		double r0 = Math::Norm(mA * mX - mF);
		double r = r0;

		Math::DMatX B(1.0, para.n, para.n); // E

		const Math::DMatX& B_inv = B;

		auto rr = mA * mX - mF;
		auto w = B_inv * rr;
		auto mAw = mA * w;
		double tau = Math::Dot(w, rr) / Math::Dot(mAw, w);

		auto preX = mX;
		mX = B_inv * (B - tau * mA) * mX + tau * mF;

		double alpha = 1.0;

		while (r > r0 * FLT_EPSILON)
		{
			mX = ConjugateGradientNextX(B, B_inv, preX, rr, w, tau, alpha);

			r = Math::Norm(mA * mX - mF);
			count++;
		}

		return count;
	}

	Math::DVecX MinimalCorrectionsNextX(const Math::DMatX& B_inv)
	{
		auto r = mA * mX - mF;
		auto w = B_inv * r;
		auto mAw = mA * w;
		double tau = Math::Dot(mAw, w) / Math::Dot(B_inv * mAw, mAw);

		return B_inv * -r * tau + mX;
	}

	Math::DVecX ConjugateGradientNextX(const Math::DMatX& B, const Math::DMatX& B_inv, Math::DVecX& prevX, Math::DVecX& preR, Math::DVecX& preW, double& preTau, double& preAlpha)
	{
		auto r = mA * mX - mF;
		auto w = B_inv * r;
		auto mAw = mA * w;
		double tau = Math::Dot(w, r) / Math::Dot(mAw, w);
		double alpha = 1.0 / (1.0 - (tau / preTau) * (Math::Dot(w, r) / Math::Dot(preW, preR)) / preAlpha);

		auto newX = B_inv * (alpha * (B - tau * mA) * mX + (1.0 - alpha) * B * prevX + alpha * tau * mF);

		prevX = mX;
		preTau = tau;
		preAlpha = alpha;
		preR = r;
		preW = w;

		return newX;
	}

private:
	Math::DMatX mA;
	Math::DVecX mF;
	Math::DVecX mX;

private:
	bool mCalculate = false;
	bool mCountIter = false;

	Para mCurrPara;
	std::vector<int> xs;
	std::vector<int> counts1;
	std::vector<int> counts2;

public:
	REGISTER_FUNC(createFunc, castFunc)
	{
		Registry::RegisterClassDe("H02_2_P05", createFunc, castFunc, "Project")
			.RegisterField("n", &H02_2_P05::n, "int", "Range(10,100)");
	}
};  RCLASS_END(H02_2_P05)
