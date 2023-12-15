#pragma once

#include "implot.h"

#include <string>
#include <algorithm>
#include <functional>

namespace PlotGUI
{
	struct PlotDescriptor
	{
		bool showTitle = true;
		ImPlotFlags plotFlags = 0;
		ImPlotAxisFlags axisFlags = 0;
		ImPlotLineFlags lineFlags = 0;
		ImPlotHeatmapFlags heatmapFlags = 0;
	};

	struct AxesLimits
	{
		double xMin;
		double xMax;
		double yMin;
		double yMax;
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
			const std::string& title, const std::string& xLabel, const std::string& yLabel,
			const T* x1, const T* y1, int count1, const std::string& name1,
			const T* x2, const T* y2, int count2, const std::string& name2,
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
				ImPlot::PlotLine(name2.c_str(), x2, y2, count2, desc.lineFlags);

				ImPlot::EndPlot();
			}
		}

		template <typename T>
		static void Plot3(
			const std::string& title,
			const T* x1, const T* y1, int count1, const std::string& name1,
			const T* x2, const T* y2, int count2, const std::string& name2,
			const T* x3, const T* y3, int count3, const std::string& name3,
			const PlotDescriptor& desc);

		template <typename T>
		static void Plot3G(
			const std::string& title, const std::string& xLabel, const std::string& yLabel, int count,
			const std::function<T(T)>& getter1, const std::string& name1,
			const std::function<T(T)>& getter2, const std::string& name2,
			const std::function<T(T)>& getter3, const std::string& name3,
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

			std::vector<double> x(count);
			std::vector<double> y1(count);
			std::vector<double> y2(count);
			std::vector<double> y3(count);

			if (ImPlot::BeginPlot(t.c_str(), { -1, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);

				auto rect = ImPlot::GetPlotLimits();

				double curr = rect.X.Min;
				const double step = (rect.X.Max - curr) / count;

				for (int i = 0; i < count; ++i)
				{
					x[i] = curr;
					y1[i] = getter1(curr);
					y2[i] = getter2(curr);
					y3[i] = getter3(curr);

					curr += step;
				}

				ImPlot::PlotLine(name1.c_str(), x.data(), y1.data(), count, desc.lineFlags);
				ImPlot::PlotLine(name2.c_str(), x.data(), y2.data(), count, desc.lineFlags);
				ImPlot::PlotLine(name3.c_str(), x.data(), y3.data(), count, desc.lineFlags);

				ImPlot::EndPlot();
			}
		}

		template <typename T>
		static void PlotShade3G(
			const std::string& title, const std::string& xLabel, const std::string& yLabel, int count,
			const std::function<T(T)>& getter1, const std::string& name1,
			const std::function<T(T)>& getter2, const std::string& name2,
			const std::function<T(T)>& getter3, const std::string& name3,
			const T* scatterX, const T* scatterY, int scatterCount, 
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

			std::vector<double> x(count);
			std::vector<double> y1(count);
			std::vector<double> y2(count);
			std::vector<double> y3(count);

			if (ImPlot::BeginPlot(t.c_str(), { -1, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);

				auto rect = ImPlot::GetPlotLimits();

				double curr = rect.X.Min;
				const double step = (rect.X.Max - curr) / count;

				for (int i = 0; i < count; ++i)
				{
					x[i] = curr;
					y1[i] = getter1(curr);
					y2[i] = getter2(curr);
					y3[i] = getter3(curr);

					curr += step;
				}

				ImPlot::PlotLine(name1.c_str(), x.data(), y1.data(), count, desc.lineFlags);
				ImPlot::PlotLine(name2.c_str(), x.data(), y2.data(), count, desc.lineFlags);
				ImPlot::PlotLine(name3.c_str(), x.data(), y3.data(), count, desc.lineFlags);

				if (scatterCount > 0)
				{
					ImPlot::PlotScatter("##Point", scatterX, scatterY, scatterCount);
				}

				ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.3f);
				ImPlot::PlotShaded("##Shade1", x.data(), y1.data(), y2.data(), count);
				ImPlot::PlotShaded("##Shade2", x.data(), y2.data(), y3.data(), count);
				ImPlot::PlotShaded("##Shade3", x.data(), y3.data(), y1.data(), count);
				ImPlot::PopStyleVar();

				ImPlot::EndPlot();
			}
		}

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

	public:
		template <typename T>
		static void PlotColorMap2D(
			const std::string& title, const std::string& xLabel, const std::string& yLabel,
			const T* data, int rows, int cols, const std::string& name,
			const PlotDescriptor& desc, const AxesLimits& limits)
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

			int min = *std::min_element(data, data + rows * cols);
			int max = *std::max_element(data, data + rows * cols);

			ImPlot::PushColormap(ImPlotColormap_Viridis);
			if (ImPlot::BeginPlot(t.c_str(), { ImGui::GetContentRegionAvail().x - 150.0f - ImGui::GetStyle().ItemSpacing.x, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);
				//ImPlot::SetupAxesLimits(limits.xMin, limits.xMax, limits.yMin, limits.yMax);

				ImPlot::PlotHeatmap(name.c_str(), data, rows, cols, min, max, NULL, ImPlotPoint(limits.xMin, limits.yMin), ImPlotPoint(limits.xMax, limits.yMax));

				ImPlot::EndPlot();
			}
			ImGui::SameLine();
			ImPlot::ColormapScale("Count", min, max, { 150, -1 }, "%g", ImPlotColormapScaleFlags_Opposite);
			ImPlot::PopColormap();
		}

	private:
		static bool NeedShow();
	};
}
