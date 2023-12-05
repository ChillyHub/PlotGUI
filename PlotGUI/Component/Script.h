#pragma once

namespace PlotGUI
{
	class Script
	{
	public:
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnChange() = 0;
		virtual void OnInspector() = 0;
		virtual void OnPlot() = 0;

	public:
		virtual ~Script() = default;
	};
}
