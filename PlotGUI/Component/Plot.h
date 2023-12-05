#pragma once

#include "implot.h"

#include <string>

namespace PlotGUI
{
	struct PlotDescriptor
	{
		bool showTitle = true;
		ImPlotFlags plotFlags = 0;
		ImPlotAxisFlags axisFlags = 0;
		ImPlotLineFlags lineFlags = 0;
	};

	class Plot
	{
	public:
		static int TotalPages;
		static int PlotPages;
		static int CurrPages;

	public:
		static void Init();
		static void Reset();

	public:
		template <typename T>
		static void Plot1(
			const std::string& title, const std::string& xLabel, const std::string& yLabel,
			const T* x1, const T* y1, int count1, const std::string& name1,
			const PlotDescriptor& desc)
		{
			if (!NeedShow())
			{
				return;
			}

			std::string t = title;
			if (!desc.showTitle)
			{
				t = std::string("##") + title;
			}

			if (ImPlot::BeginPlot(t.c_str(), { -1, -1 }))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);

				ImPlot::PlotLine(name1.c_str(), x1, y1, count1, desc.lineFlags);

				ImPlot::EndPlot();
			}
		}

		template <typename T>
		static void Plot2(
			const std::string& title,
			const T* x1, const T* y1, int count1, const std::string& name1,
			const T* x2, const T* y2, int count2, const std::string& name2,
			const PlotDescriptor& desc);

		template <typename T>
		static void Plot3(
			const std::string& title,
			const T* x1, const T* y1, int count1, const std::string& name1,
			const T* x2, const T* y2, int count2, const std::string& name2,
			const T* x3, const T* y3, int count3, const std::string& name3,
			const PlotDescriptor& desc);

		template <typename T>
		static void PlotN(
			const std::string& title,
			const T* x1, const T* y1, int* count, const std::string* names, int n,
			const PlotDescriptor& desc);

	public:
		template <typename T>
		static void PlotHistogram1(
			const std::string& title, 
			const std::string& name1, const T* y, int count, 
			const PlotDescriptor& desc)
		{
			if (!NeedShow())
			{
				return;
			}

			std::string t = title;
			if (!desc.showTitle)
			{
				t = std::string("##") + title;
			}

			if (ImPlot::BeginPlot(t.c_str(), { -1, -1 }))
			{
				ImPlot::PlotHistogram(name1.c_str(), y, count);

				ImPlot::EndPlot();
			}
		}

	private:
		static bool NeedShow();
	};
}
