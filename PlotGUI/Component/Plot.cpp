#include "Plot.h"

namespace PlotGUI
{
	int Plot::TotalPages = 0;
	int Plot::PlotPages = 0;
	int Plot::CurrPages = 1;

	void Plot::Reset()
	{
		PlotPages = 0;
	}

	void Plot::Plot1(const std::string& title, const float* x1, const float* y1, int count1, const std::string& name1, const PlotDescriptor& desc)
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

        if (ImPlot::BeginPlot(t.c_str(), {-1, -1}))
        {
            ImPlot::PlotLine(name1.c_str(), x1, y1, count1);

            ImPlot::EndPlot();
        }
	}

	bool Plot::NeedShow()
	{
		PlotPages++;
		TotalPages = PlotPages > TotalPages ? PlotPages : TotalPages;
		if (PlotPages == CurrPages)
		{
			return true;
		}

		return false;
	}
}
