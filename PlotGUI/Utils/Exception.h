#pragma once

#include <string>
#include <exception>

#ifdef WIN32
	#include <Windows.h>
#endif

namespace PlotGUI
{
    class PlotException : public std::exception
    {
    public:
        PlotException() = default;
        PlotException(long hr, const std::wstring& functionName, const std::wstring& filename, int lineNumber);

        std::wstring ToString() const;
        char const* what() const override;

    public:
        static std::wstring AnsiToWString(const std::string& str);

    private:
        long m_ErrorCode = 0;
        std::wstring m_FunctionName;
        std::wstring m_Filename;
        int m_LineNumber = -1;
    };

#ifndef ThrowIfFailed
    #define ThrowIfFailed(x)                                                 \
        {                                                                    \
            long hr__ = (x);                                                 \
            std::wstring wfn = PlotException::AnsiToWString(__FILE__);       \
            if (hr__ < 0) { throw PlotException(hr__, L#x, wfn, __LINE__); } \
        }
#endif

    #define ThrowRuntimeError(x)                                               \
    	throw std::runtime_error(std::string(                                  \
    	"ERROR: [File: ") + __FILE__ + " Line: " + std::to_string(__LINE__) +  \
		" " + std::string(x)) 
}
