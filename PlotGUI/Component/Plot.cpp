#include "Plot.h"

namespace PlotGUI
{
	int Plot::TotalPages = 0;
	int Plot::PlotPages = 0;
	int Plot::CurrPages = 1;

	void Plot::Init()
	{
		TotalPages = 0;
		PlotPages = 0;
		CurrPages = 1;
	}

	void Plot::Reset()
	{
		PlotPages = 0;
	}

	bool Plot::NeedShow()
	{
		PlotPages++;
		CurrPages = PlotPages > TotalPages ? PlotPages : CurrPages;
		TotalPages = PlotPages > TotalPages ? PlotPages : TotalPages;
		if (PlotPages == CurrPages)
		{
			return true;
		}

		return false;
	}
}
