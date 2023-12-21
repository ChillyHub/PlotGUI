#pragma once

#include "Joiner.h"
#include "ThreadQueue.h"
#include "FunctionWrapper.h"

#include <atomic>
#include <future>
#include <functional>

namespace PlotGUI
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(uint32_t threadsNum = std::thread::hardware_concurrency())
            : mDone(false), mJoiner(mThreads)
        {
            try
            {
                for (unsigned int i = 0; i < threadsNum; ++i)
                {
                    mThreads.emplace_back(&ThreadPool::workThread, this);
                }
            }
            catch (const std::exception& e)
            {
                mDone = true;
                throw std::runtime_error("Create threads failed.");
            }
        }

        ~ThreadPool()
        {
            WaitAndStop();
        }

        template <typename Func, typename ... Args>
        std::future<std::invoke_result_t<Func, Args...>> Submit(Func&& func, Args&&... args)
        {
            using ResType = std::invoke_result_t<Func, Args...>;
            auto task = std::make_shared<std::packaged_task<ResType()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
            std::future<ResType> res(task->get_future());
            mQueue.Push([task]() { (*task)(); });
            return res;
        }

        void Stop()
        {
            mDone = true;
        }

        void WaitAndStop()
        {
            while (!mQueue.IsEmpty());
            mDone = true;
        }

    private:
        void workThread()
        {
            while (!mDone)
            {
                FunctionWrapper task;
                if (mQueue.TryPop(task))
                {
                    task();
                }
                else
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
        }

    private:
        std::atomic<bool> mDone;
        std::vector<std::thread> mThreads;
        ThreadQueue<FunctionWrapper> mQueue;
        Joiner mJoiner;
    };
}
