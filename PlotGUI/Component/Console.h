#pragma once

#include "PlotGUI/Utils/Utils.h"

#include "ThirdPartys/Imgui/imgui.h"

namespace PlotGUI
{
    class Console : public Singleton<Console>
    {
    public:
	    enum class LogType
	    {
		    None,
            Info,
            Warning,
            Error,
            Command
	    };

    public:
        static void EnableLogToTerminal(bool b = true);
        static void Log(const char* str, ...) IM_FMTARGS(2);
        static void Log(const std::string& str);
        static void LogInfo(const char* str, ...) IM_FMTARGS(2);
        static void LogInfo(const std::string& str);
        static void LogWarning(const char* str, ...) IM_FMTARGS(2);
        static void LogWarning(const std::string& str);
        static void LogError(const char* str, ...) IM_FMTARGS(2);
        static void LogError(const std::string& str);

    private:
        // Portable helpers
        static int   Stricmp(const char* s1, const char* s2);
        static int   Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void  Strtrim(char* s);
        static char* Strcat(const char* l, const char* r);

        // In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);

    public:
        Console();
        virtual ~Console() override;

    public:
        void Draw(const char* title, bool* p_open);

    private:
        void ClearLog();
        void AddLog(const char* fmt, ...) IM_FMTARGS(2);
        void AddLog(const char* fmt, va_list args);
        void AddLogInfo(const char* fmt, ...) IM_FMTARGS(2);
        void AddLogInfo(const char* fmt, va_list va);
        void AddLogWarning(const char* fmt, ...) IM_FMTARGS(2);
        void AddLogWarning(const char* fmt, va_list va);
        void AddLogError(const char* fmt, ...) IM_FMTARGS(2);
        void AddLogError(const char* fmt, va_list va);
        void AddLogCommand(const char* fmt, ...) IM_FMTARGS(2);
        void AddLogCommand(const char* fmt, va_list va);
        void ExecCommand(const char* command_line);

        int TextEditCallback(ImGuiInputTextCallbackData* data);

    private:
        char                  m_inputBuf[256];
        ImVector<char*>       m_items;
        ImVector<LogType>     m_itemTypes;
        ImVector<const char*> m_commands;
        ImVector<char*>       m_history;
        int                   m_historyPos;    // -1: new line, 0..m_history.Size-1 browsing history.
        ImGuiTextFilter       m_filter;
        bool                  m_autoScroll;
        bool                  m_scrollToBottom;

        bool                  m_showInfo = true;
        bool                  m_showWarning = true;
        bool                  m_showError = true;

    private:
        static bool           s_logToTerminal;
    };
}
