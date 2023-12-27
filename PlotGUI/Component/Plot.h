#pragma once

#include "implot.h"

#include <string>
#include <algorithm>
#include <functional>

#include <cmath>

namespace PlotGUI
{
	struct PlotDescriptor
	{
		bool showTitle = true;
		ImPlotFlags plotFlags = 0;
		ImPlotSubplotFlags subplotFlags = 0;
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

				if (count1 > 0)
				{
					ImPlot::PlotLine(name1.c_str(), x1, y1, count1, desc.lineFlags);
				}

				ImPlot::EndPlot();
			}
		}

		template <typename T>
		static void Plot1G(
			const std::string& title, const std::string& xLabel, const std::string& yLabel, int count, 
			const std::function<T(T)>& getter1, const std::string& name1,
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
			std::vector<double> y(count);

			if (ImPlot::BeginPlot(t.c_str(), { -1, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);

				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 3.0);

				auto rect = ImPlot::GetPlotLimits();

				double curr = rect.X.Min;
				const double step = (rect.X.Max - curr) / count;

				for (int i = 0; i < count; ++i)
				{
					x[i] = curr;
					y[i] = getter1(curr);

					curr += step;
				}

				ImPlot::PlotLine(name1.c_str(), x.data(), y.data(), count, desc.lineFlags);

				if (scatterCount > 0)
				{
					ImPlot::PlotScatter("##Point", scatterX, scatterY, scatterCount);
				}

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
		static void Plot2G(
			const std::string& title, const std::string& xLabel, const std::string& yLabel, int count,
			const std::function<T(T)>& getter1, const std::string& name1,
			const std::function<T(T)>& getter2, const std::string& name2,
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

					curr += step;
				}

				ImPlot::PlotLine(name1.c_str(), x.data(), y1.data(), count, desc.lineFlags);
				ImPlot::PlotLine(name2.c_str(), x.data(), y2.data(), count, desc.lineFlags);

				ImPlot::EndPlot();
			}
		}

		template <typename T>
		static void Plot3(
			const std::string& title, const std::string& xLabel, const std::string& yLabel, 
			const T* x1, const T* y1, int count1, const std::string& name1,
			const T* x2, const T* y2, int count2, const std::string& name2,
			const T* x3, const T* y3, int count3, const std::string& name3,
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
				ImPlot::PlotLine(name3.c_str(), x3, y3, count3, desc.lineFlags);

				ImPlot::EndPlot();
			}
		}

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
		static void Plot3G(
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

		template <typename T>
		static void Subplots2_2G2G(
			const std::string& title,
			const std::string& subtitle1, const std::string& xLabel1, const std::string& yLabel1, int count1,
			const std::function<T(T)>& getter11, const std::string& name11,
			const std::function<T(T)>& getter12, const std::string& name12,
			const T* scatterX1, const T* scatterY1, int scatterCount1,
			const std::string& subtitle2, const std::string& xLabel2, const std::string& yLabel2, int count2,
			const std::function<T(T)>& getter21, const std::string& name21,
			const std::function<T(T)>& getter22, const std::string& name22,
			const T* scatterX2, const T* scatterY2, int scatterCount2,
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

			if (ImPlot::BeginSubplots(t.c_str(), 1, 2, { -1, -1 }, desc.subplotFlags))
			{
				if (ImPlot::BeginPlot(subtitle1.c_str(), { -1, -1 }, desc.plotFlags))
				{
					ImPlot::SetupAxes(xLabel1.c_str(), yLabel1.c_str(), desc.axisFlags, desc.axisFlags);

					auto rect = ImPlot::GetPlotLimits();

					double curr = rect.X.Min;
					const double step = (rect.X.Max - curr) / count1;

					std::vector<double> x(count1);
					std::vector<double> y1(count1);
					std::vector<double> y2(count1);

					for (int i = 0; i < count1; ++i)
					{
						x[i] = curr;
						y1[i] = getter11(curr);
						y2[i] = getter12(curr);

						curr += step;
					}

					ImPlot::PlotLine(name11.c_str(), x.data(), y1.data(), count1, desc.lineFlags);
					ImPlot::PlotLine(name12.c_str(), x.data(), y2.data(), count1, desc.lineFlags);

					if (scatterCount1 > 0)
					{
						ImPlot::PlotScatter("##Point1", scatterX1, scatterY1, scatterCount1);
					}

					ImPlot::EndPlot();
				}

				if (ImPlot::BeginPlot(subtitle2.c_str(), { -1, -1 }, desc.plotFlags))
				{
					ImPlot::SetupAxes(xLabel2.c_str(), yLabel2.c_str(), desc.axisFlags, desc.axisFlags);

					auto rect = ImPlot::GetPlotLimits();

					double curr = rect.X.Min;
					const double step = (rect.X.Max - curr) / count2;

					std::vector<double> x(count2);
					std::vector<double> y1(count2);
					std::vector<double> y2(count2);

					for (int i = 0; i < count2; ++i)
					{
						x[i] = curr;
						y1[i] = getter21(curr);
						y2[i] = getter22(curr);

						curr += step;
					}

					ImPlot::PlotLine(name21.c_str(), x.data(), y1.data(), count2, desc.lineFlags);
					ImPlot::PlotLine(name22.c_str(), x.data(), y2.data(), count2, desc.lineFlags);

					if (scatterCount2 > 0)
					{
						ImPlot::PlotScatter("##Point1", scatterX2, scatterY2, scatterCount2);
					}

					ImPlot::EndPlot();
				}

				ImPlot::EndSubplots();
			}
		}

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

			double min = 0.0;
			double max = 1.0;

			ImPlot::PushColormap(ImPlotColormap_Viridis);
			if (ImPlot::BeginPlot(t.c_str(), { ImGui::GetContentRegionAvail().x - 150.0f - ImGui::GetStyle().ItemSpacing.x, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);
				//ImPlot::SetupAxesLimits(limits.xMin, limits.xMax, limits.yMin, limits.yMax);

				if (rows * cols)
				{
					min = *std::min_element(data, data + rows * cols);
					max = *std::max_element(data, data + rows * cols);

					ImPlot::PlotHeatmap(name.c_str(), data, rows, cols, min, max, NULL, ImPlotPoint(limits.xMin, limits.yMin), ImPlotPoint(limits.xMax, limits.yMax));
				}

				ImPlot::EndPlot();
			}
			ImGui::SameLine();
			ImPlot::ColormapScale("Count", min, max, { 150, -1 }, "%g", ImPlotColormapScaleFlags_Opposite);
			ImPlot::PopColormap();
		}

		template <typename T>
		static void PlotColorMap2DL(
			const std::string& title, const std::string& xLabel, const std::string& yLabel,
			const T* data, int rows, int cols, int drawRows, int drawCols, const std::string& name,
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

			double min = 0.0;
			double max = 1.0;

			ImPlot::PushColormap(ImPlotColormap_Viridis);
			if (ImPlot::BeginPlot(t.c_str(), { ImGui::GetContentRegionAvail().x - 150.0f - ImGui::GetStyle().ItemSpacing.x, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);
				//ImPlot::SetupAxesLimits(limits.xMin, limits.xMax, limits.yMin, limits.yMax);

				if (rows * cols > 3)
				{
					std::vector<double> dat(rows * cols);

					for (int i = 0; i < rows / 2 + 1; ++i)
					{
						for (int j = 0; j < rows; ++j)
						{
							dat[i * rows + j] = data[(rows - 1 - i) * rows + j];
							dat[(rows - 1 - i) * rows + j] = data[i * rows + j];
						}
					}

					double rateR = static_cast<double>(rows - 1) / drawRows;
					double rateC = static_cast<double>(cols - 1) / drawCols;

					std::vector<double> plotData(drawRows * drawCols, 0.0);

					auto lerp = [](double a, double b, double t) { return a * (1.0 - t) + b * t; };

					for (int i = 0; i < drawRows; ++i)
					{
						for (int j = 0; j < drawCols; ++j)
						{
							int index = i * drawCols + j;

							double ii = i + 0.5;
							double jj = j + 0.5;

							int sx1 = static_cast<int>(std::floor(ii * rateR));
							int sy1 = static_cast<int>(std::floor(jj * rateC));
							int sx2 = std::min(sx1 + 1, rows - 1);
							int sy2 = std::min(sy1 + 1, cols - 1);

							double lx = ii * rateR - sx1;
							double ly = jj * rateC - sy1;

							plotData[index] = lerp(
								lerp(dat[sy1 * rows + sx1], dat[sy1 * rows + sx2], lx),
								lerp(dat[sy2 * rows + sx1], dat[sy2 * rows + sx2], lx),
								ly);
						}
					}

					min = *std::min_element(plotData.begin(), plotData.end());
					max = *std::max_element(plotData.begin(), plotData.end());

					ImPlot::PlotHeatmap(name.c_str(), plotData.data(), drawRows, drawCols, min, max, NULL, ImPlotPoint(limits.xMin, limits.yMin), ImPlotPoint(limits.xMax, limits.yMax));
				}

				ImPlot::EndPlot();
			}
			ImGui::SameLine();
			ImPlot::ColormapScale("Count", min, max, { 150, -1 }, "%g", ImPlotColormapScaleFlags_Opposite);
			ImPlot::PopColormap();
		}

		template <typename T>
		static void PlotColorMap2DG(
			const std::string& title, const std::string& xLabel, const std::string& yLabel,
			const std::function<T(T, T)>& getter, int rows, int cols, const std::string& name,
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

			std::vector<T> data(rows * cols);
			T min = T(0);
			T max = T(1);

			ImPlot::PushColormap(ImPlotColormap_Viridis);
			if (ImPlot::BeginPlot(t.c_str(), { ImGui::GetContentRegionAvail().x - 150.0f - ImGui::GetStyle().ItemSpacing.x, -1 }, desc.plotFlags))
			{
				ImPlot::SetupAxes(xLabel.c_str(), yLabel.c_str(), desc.axisFlags, desc.axisFlags);

				// Generate data
				auto rect = ImPlot::GetPlotLimits();

				double currX = rect.X.Min;
				const double stepX = (rect.X.Max - currX) / cols;

				double currY = rect.Y.Min;
				const double stepY = (rect.Y.Max - currY) / rows;

				for (int i = rows - 1; i >= 0; --i)
				{
					for (int j = 0; j < cols; ++j)
					{
						data[i * cols + j] = getter(currX + stepX * j, currY + stepY * (rows - i - 1));
					}
				}

				min = *std::min_element(data.begin(), data.end());
				max = *std::max_element(data.begin(), data.end());

				// Draw
				ImPlot::PlotHeatmap(name.c_str(), data.data(), rows, cols, min, max, NULL, ImPlotPoint(rect.X.Min, rect.Y.Min), ImPlotPoint(rect.X.Max, rect.Y.Max));

				ImPlot::EndPlot();
			}
			ImGui::SameLine();
			ImPlot::ColormapScale("y", min, max, { 150, -1 }, "%g", ImPlotColormapScaleFlags_Opposite);
			ImPlot::PopColormap();
		}

	private:
		static bool NeedShow();
	};
}
