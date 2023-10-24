#pragma once

namespace PlotGUI
{
	class GuiWindow
	{
	public:
		virtual ~GuiWindow() = default;
	public:
		virtual void InitWindow() = 0;
		virtual bool PollEvent() = 0;
		virtual void Resize() = 0;
		virtual void StartFrame() = 0;
		virtual void Present() = 0;
		virtual void Destroy() = 0;

		unsigned int GetWidth() { return s_resizeWidth; }
		unsigned int GetHeight() { return s_resizeHeight; }

	protected:
		static unsigned int s_resizeWidth;
		static unsigned int s_resizeHeight;
	};
}
