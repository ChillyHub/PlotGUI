#pragma once

#include <chrono>

#include <string>
#include <unordered_map>

namespace PlotGUI
{
	struct ProfilerInfo
	{
		double startTime;
		double endTime;
		double deltaTime;
	};

	class Profiler
	{
	public:
		static std::unordered_map<std::string, ProfilerInfo> Infos;

	public:
		static ProfilerInfo GetProfileInfo(const std::string& infoName);

	public:
		Profiler(const std::string& profilerName);
		~Profiler();

	private:
		std::string mProfilerName;
		ProfilerInfo mProfilerInfo;
	};
}

#define ProfilerBegin(name, profiler) PlotGUI::Profiler profiler = PlotGUI::Profiler(name)
#define ProfilerEnd(name, profiler) profiler.~Profiler()
#define Profile(name) PlotGUI::Profiler profiler = PlotGUI::Profiler(name)
