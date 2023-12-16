#pragma once

#include "Utils/Singleton.h"
#include "Thread/ThreadPool.h"

namespace PlotGUI
{
	class Threads : public Singleton<Threads>
	{
	public:
		template <typename Func, typename ... Args>
		static std::future<std::result_of_t<Func(Args...)>> Run(Func&& func, Args&&... args)
		{
			if (!Instance().mThreadPool)
			{
				Instance().mThreadPool = std::make_unique<ThreadPool>();
			}

			return Instance().mThreadPool->Submit(std::forward<Func>(func), std::forward<Args>(args)...);
		}

		static void Stop()
		{
			if (!Instance().mThreadPool)
			{
				Instance().mThreadPool = std::make_unique<ThreadPool>();
			}

			Instance().mThreadPool->Stop();
		}

	private:
		std::unique_ptr<ThreadPool> mThreadPool;
	};
}
