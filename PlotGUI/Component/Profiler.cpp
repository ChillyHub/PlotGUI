#include "Component/Profiler.h"

namespace PlotGUI
{
	std::unordered_map<std::string, ProfilerInfo> Profiler::Infos = std::unordered_map<std::string, ProfilerInfo>();

	ProfilerInfo Profiler::GetProfileInfo(const std::string& infoName)
	{
		ProfilerInfo info;

		if (Infos.find(infoName) != Infos.end())
		{
			info = Infos[infoName];
		}

		return info;
	}

	Profiler::Profiler(const std::string& profilerName)
	{
		mProfilerName = profilerName;
		mProfilerInfo.startTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count() * 0.001;
	}

	Profiler::~Profiler()
	{
		mProfilerInfo.endTime = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()).time_since_epoch().count() * 0.001;
		mProfilerInfo.deltaTime = mProfilerInfo.endTime - mProfilerInfo.startTime;

		Infos[mProfilerName] = mProfilerInfo;
	}
}
