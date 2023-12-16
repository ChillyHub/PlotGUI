#pragma once

#include <vector>
#include <thread>

namespace PlotGUI
{
    class Joiner
    {
    public:
        explicit Joiner(std::vector<std::thread>& threads) : mThreads(threads) {}

        ~Joiner()
        {
            Join();
        }

        void Join()
        {
            for (auto &&thread : mThreads)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            } 
        }
    private:
        std::vector<std::thread>& mThreads;
    };
}
