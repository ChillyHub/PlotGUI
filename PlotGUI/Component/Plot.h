#pragma once

#include "implot.h"

#include <string>

namespace PlotGUI
{
	struct PlotDescriptor
	{
		bool showTitle = true;
		ImPlotLineFlags flags = 0;
	};

	class Plot
	{
	public:
		static int TotalPages;
		static int PlotPages;
		static int CurrPages;

	public:
		static void Reset();

	public:
		static void Plot1(
			const std::string& title, 
			const float* x1, const float* y1, int count1, const std::string& name1, 
			const PlotDescriptor& desc);

		static void Plot2(
			const std::string& title,
			const float* x1, const float* y1, int count1, const std::string& name1,
			const float* x2, const float* y2, int count2, const std::string& name2,
			const PlotDescriptor& desc);

		static void Plot3(
			const std::string& title,
			const float* x1, const float* y1, int count1, const std::string& name1,
			const float* x2, const float* y2, int count2, const std::string& name2,
			const float* x3, const float* y3, int count3, const std::string& name3,
			const PlotDescriptor& desc);

		static void PlotN(
			const std::string& title,
			const float* x1, const float* y1, int* count, const std::string* names, int n,
			const PlotDescriptor& desc);

	private:
		static bool NeedShow();
	};
}
