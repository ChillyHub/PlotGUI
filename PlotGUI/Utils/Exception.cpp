#include "Exception.h"

#ifdef _MSC_VER
	#include "comdef.h"
#endif

namespace PlotGUI
{
	PlotException::PlotException(long hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber)
		: m_ErrorCode(hr), m_FunctionName(functionName), m_Filename(filename), m_LineNumber(lineNumber)
	{
	}

	std::wstring PlotException::ToString() const
	{
#ifdef _MSC_VER
		// Get the string description of the error code.
		_com_error err(m_ErrorCode);
		std::wstring msg = err.ErrorMessage();
#else
		std::wstring msg = L"Unknown";
#endif

		return m_FunctionName + L" failed in " + m_Filename + L"; line " + 
			std::to_wstring(m_LineNumber) + L"; error: " + msg;
	}

	char const* PlotException::what() const
	{
		return "Unknown";
	}

	std::wstring PlotException::AnsiToWString(const std::string& str)
	{
#ifdef WIN32
		WCHAR buffer[512];
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
		return std::wstring(buffer);
#else
		return std::wstring("Unknown");
#endif
	}
}
