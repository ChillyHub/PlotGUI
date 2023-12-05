#include "Console.h"

#include "misc/fonts/IconsFontAwesome6.h"

#include <iostream>

namespace PlotGUI
{
    bool Console::s_logToTerminal = false;

    void Console::EnableLogToTerminal(bool b)
    {
        s_logToTerminal = b;
    }

	void Console::Log(const char* str, ...)
	{
        va_list args;
        va_start(args, str);
        Instance().AddLog(str, args);
        va_end(args);
	}

	void Console::Log(const std::string& str)
	{
        Log(str.c_str());
	}

	void Console::LogInfo(const char* str, ...)
	{
        va_list args;
        va_start(args, str);
        Instance().AddLogInfo(str, args);
        va_end(args);
	}

	void Console::LogInfo(const std::string& str)
	{
        LogInfo(str.c_str());
	}

	void Console::LogWarning(const char* str, ...)
	{
        va_list args;
        va_start(args, str);
        Instance().AddLogWarning(str, args);
        va_end(args);
	}

	void Console::LogWarning(const std::string& str)
	{
        LogWarning(str.c_str());
	}

	void Console::LogError(const char* str, ...)
	{
        va_list args;
        va_start(args, str);
        Instance().AddLogError(str, args);
        va_end(args);
	}

	void Console::LogError(const std::string& str)
	{
        LogError(str.c_str());
	}

	int Console::Stricmp(const char* s1, const char* s2)
	{
		int d;
		while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
		{
			s1++;
			s2++;
		}
		return d;
	}

	int Console::Strnicmp(const char* s1, const char* s2, int n)
	{
		int d = 0;
		while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
		{
			s1++;
			s2++;
			n--;
		}
		return d;
	}

	char* Console::Strdup(const char* s)
	{
		IM_ASSERT(s);
		size_t len = strlen(s) + 1;
		void* buf = malloc(len);
		IM_ASSERT(buf);
		return (char*)memcpy(buf, (const void*)s, len);
	}

	void Console::Strtrim(char* s)
	{
		char* str_end = s + strlen(s);
		while (str_end > s && str_end[-1] == ' ') 
			str_end--;
		*str_end = 0;
	}

	char* Console::Strcat(const char* l, const char* r)
	{
        IM_ASSERT(l);
        IM_ASSERT(r);
        size_t lenL = strlen(l);
        size_t lenR = strlen(r);

        void* buf = malloc(lenL + lenR + 1);
        IM_ASSERT(buf);
        memcpy(buf, (const void*)l, lenL);
        memcpy((void*)(((char*)buf) + lenL), (const void*)r, lenR + 1);
        return (char*)buf;
	}

	int Console::TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
        Console* console = (Console*)data->UserData;
        return console->TextEditCallback(data);
	}

	Console::Console()
		: Singleton<PlotGUI::Console>()
	{
		ClearLog();
		memset(m_inputBuf, 0, sizeof(m_inputBuf));
		m_historyPos = -1;

		m_autoScroll = true;
		m_scrollToBottom = false;
		AddLogInfo("Plot GUI! START!");
	}

	Console::~Console()
	{
		ClearLog();
		for (int i = 0; i < m_history.Size; i++)
			free(m_history[i]);
	}

	void Console::ClearLog()
	{
		for (int i = 0; i < m_items.Size; i++)
			free(m_items[i]);
		m_items.clear();
        m_itemTypes.clear();
	}

	void Console::AddLog(const char* fmt, ...)
	{
		// FIXME-OPT
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		m_items.push_back(Strdup(buf));
        m_itemTypes.push_back(LogType::None);

        if (s_logToTerminal)
        {
            std::cout << m_items.back() << std::endl;
        }
	}

	void Console::AddLogInfo(const char* fmt, ...)
	{
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        m_items.push_back(Strcat("[Info]:    ", buf));
        m_itemTypes.push_back(LogType::Info);

        if (s_logToTerminal)
        {
            std::cout << m_items.back() << std::endl;
        }
	}

	void Console::AddLogWarning(const char* fmt, ...)
	{
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        m_items.push_back(Strcat("[Warning]: ", buf));
        m_itemTypes.push_back(LogType::Warning);

        if (s_logToTerminal)
        {
            std::cout << m_items.back() << std::endl;
        }
	}

	void Console::AddLogError(const char* fmt, ...)
	{
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        m_items.push_back(Strcat("[Error]:   ", buf));
        m_itemTypes.push_back(LogType::Error);

        if (s_logToTerminal)
        {
            std::cout << m_items.back() << std::endl;
        }
	}

	void Console::AddLogCommand(const char* fmt, ...)
	{
        // FIXME-OPT
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);
        m_items.push_back(Strdup(buf));
        m_itemTypes.push_back(LogType::Command);

        if (s_logToTerminal)
        {
            std::cout << m_items.back() << std::endl;
        }
	}

	void Console::Draw(const char* title, bool* p_open)
	{
        if (ImGui::Button(" " ICON_FA_DELETE_LEFT " Clear")) { ClearLog(); }
        ImGui::SameLine();

        bool copy_to_clipboard = ImGui::Button(" " ICON_FA_COPY " Copy ");
        ImGui::SameLine();

        // Options menu
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &m_autoScroll);
            ImGui::Checkbox("Log to terminal", &s_logToTerminal);
            ImGui::EndPopup();
        }
        if (ImGui::Button(" Options " ICON_FA_CARET_DOWN " "))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();

        // Info/Warning/Error filter
        const char* filterText = ICON_FA_MAGNIFYING_GLASS;
        const char* infoButton = ICON_FA_CIRCLE_INFO;
        const char* warningButton = ICON_FA_CIRCLE_EXCLAMATION;
        const char* errorButton = ICON_FA_CIRCLE_XMARK;

        float filterWidth = 180.0f;
        float filterTextWidht = ImGui::CalcTextSize(filterText).x;
        float infoWidth = ImGui::CalcTextSize(infoButton).x;
        float warningWidth = ImGui::CalcTextSize(warningButton).x;
        float errorWidth = ImGui::CalcTextSize(errorButton).x;
        float totalWidth = filterWidth + filterTextWidht + infoWidth + warningWidth + errorWidth;

        //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
        auto posX = (ImGui::GetCursorPosX() + ImGui::GetColumnWidth() - totalWidth
            - ImGui::GetScrollX() - 2 * 3 * ImGui::GetStyle().ItemSpacing.x);
        ImGui::SetCursorPosX(posX);
        //ImGui::PopStyleVar();

        m_filter.Draw(filterText, filterWidth);
        ImGui::SameLine();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 2.0f, 2.0f });

        bool tmp = m_showInfo;
        if (m_showInfo)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 0.0f, 1.0f, 0.0f, 1.0f });
        }
        if (ImGui::Button(infoButton))
        {
            m_showInfo = !m_showInfo;
        }
        if (tmp)
        {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();

        tmp = m_showWarning;
        if (m_showWarning)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 1.0f, 0.0f, 1.0f });
        }
        if (ImGui::Button(warningButton))
        {
            m_showWarning = !m_showWarning;
        }
        if (tmp)
        {
            ImGui::PopStyleColor();
        }
        ImGui::SameLine();

        tmp = m_showError;
        if (m_showError)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, { 1.0f, 0.0f, 0.0f, 1.0f });
        }
        if (ImGui::Button(errorButton))
        {
            m_showError = !m_showError;
        }
        if (tmp)
        {
            ImGui::PopStyleColor();
        }
        ImGui::Separator();

        ImGui::PopStyleVar();

        // Reserve enough left-over height for 1 separator + 1 input text
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
        {
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable(ICON_FA_DELETE_LEFT " Clear ", false, ImGuiSelectableFlags_AllowOverlap)) ClearLog();
                ImGui::EndPopup();
            }

            // Display every line as a separate entry so we can change their color or add custom widgets.
            // If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
            // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping
            // to only process visible items. The clipper will automatically measure the height of your first item and then
            // "seek" to display only items in the visible area.
            // To use the clipper we can replace your standard loop:
            //      for (int i = 0; i < m_items.Size; i++)
            //   With:
            //      ImGuiListClipper clipper;
            //      clipper.Begin(m_items.Size);
            //      while (clipper.Step())
            //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
            // - That your items are evenly spaced (same height)
            // - That you have cheap random access to your elements (you can access them given their index,
            //   without processing all the ones before)
            // You cannot this code as-is if a filter is active because it breaks the 'cheap random-access' property.
            // We would need random-access on the post-filtered list.
            // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices
            // or offsets of items that passed the filtering test, recomputing this array when user changes the filter,
            // and appending newly elements as they are inserted. This is left as a task to the user until we can manage
            // to improve this example code!
            // If your items are of variable height:
            // - Split them into same height items would be simpler and facilitate random-seeking into your list.
            // - Consider using manual call to IsRectVisible() and skipping extraneous decoration from your items.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            if (copy_to_clipboard)
                ImGui::LogToClipboard();
            for (int i = 0; i < m_items.size(); ++i)
            {
                const char* item = m_items[i];
                LogType logType = m_itemTypes[i];

                // Normally you would store more information in your item than just a string.
                // (e.g. make m_items[] an array of structure, store color/type etc.)
                ImVec4 color;
                bool has_color = false;
                if (logType == LogType::Info)
                {
	                if (!m_showInfo)
	                {
		                continue;
	                }

                    color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
                    has_color = true;
                }
                else if (logType == LogType::Warning)
                {
                    if (!m_showWarning)
                    {
                        continue;
                    }

                    color = ImVec4(0.8f, 0.8f, 0.0f, 1.0f);
                    has_color = true;
                }
                else if (logType == LogType::Error)
                {
                    if (!m_showError)
                    {
                        continue;
                    }

	                color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                	has_color = true;
                }
                else if (logType == LogType::Command)
                {
	                color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
                	has_color = true;
                }

                if (!m_filter.PassFilter(item))
                    continue;

                auto font = ImGui::GetIO().Fonts->Fonts[1];

                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::PushFont(font);
                ImGui::TextUnformatted(item);
                ImGui::PopFont();
                if (has_color)
                    ImGui::PopStyleColor();
            }
            if (copy_to_clipboard)
                ImGui::LogFinish();

            // Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
            // Using a scrollbar or mouse-wheel will take away from the bottom edge.
            if (m_scrollToBottom || (m_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            m_scrollToBottom = false;

            ImGui::PopStyleVar();
        }
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText(" " ICON_FA_INBOX " Input ", m_inputBuf, IM_ARRAYSIZE(m_inputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
        {
            char* s = m_inputBuf;
            Strtrim(s);
            if (s[0])
                ExecCommand(s);
            strcpy(s, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}

	void Console::ExecCommand(const char* command_line)
	{
        AddLogCommand("# %s\n", command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back.
        // This isn't trying to be smart or optimal.
        m_historyPos = -1;
        for (int i = m_history.Size - 1; i >= 0; i--)
            if (Stricmp(m_history[i], command_line) == 0)
            {
                free(m_history[i]);
                m_history.erase(m_history.begin() + i);
                break;
            }
        m_history.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, "CLEAR") == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, "HELP") == 0)
        {
            AddLog("Commands:");
            for (int i = 0; i < m_commands.Size; i++)
                AddLog("- %s", m_commands[i]);
        }
        else if (Stricmp(command_line, "HISTORY") == 0)
        {
            int first = m_history.Size - 10;
            for (int i = first > 0 ? first : 0; i < m_history.Size; i++)
                AddLog("%3d: %s\n", i, m_history[i]);
        }
        else
        {
            AddLog("Unknown command: '%s'\n", command_line);
        }

        // On command input, we scroll to bottom even if m_autoScroll==false
        m_scrollToBottom = true;
	}

	int Console::TextEditCallback(ImGuiInputTextCallbackData* data)
	{
        //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (int i = 0; i < m_commands.Size; i++)
                if (Strnicmp(m_commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(m_commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                AddLog("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    AddLog("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = m_historyPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (m_historyPos == -1)
                    m_historyPos = m_history.Size - 1;
                else if (m_historyPos > 0)
                    m_historyPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (m_historyPos != -1)
                    if (++m_historyPos >= m_history.Size)
                        m_historyPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != m_historyPos)
            {
                const char* history_str = (m_historyPos >= 0) ? m_history[m_historyPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
        }
        return 0;
	}
}
