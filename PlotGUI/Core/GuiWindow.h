#pragma once

namespace PlotGUI
{
	enum class WindowEvent
	{
		Run,
		Pause,
		Quit,
	};

	class GuiWindow
	{
	public:
		virtual ~GuiWindow() = default;
	public:
		virtual void InitWindow() = 0;
		virtual void Resize() = 0;
		virtual void StartFrame() = 0;
		virtual void Present() = 0;
		virtual void Destroy() = 0;

		virtual WindowEvent PollEvent() = 0;

		unsigned int GetWidth() { return m_resizeWidth; }
		unsigned int GetHeight() { return m_resizeHeight; }

	protected:
		unsigned int m_resizeWidth = 0;
		unsigned int m_resizeHeight = 0;

		bool mAppPaused = false;
		bool mMinimized = false;
		bool mMaximized = false;
		bool mResizing = false;
		bool mFullscreenState = false;
	};
}
