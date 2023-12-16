#pragma once

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

namespace PlotGUI
{
    template <typename T>
    class ThreadQueue
    {
    public:
        void Push(T value)
        {
            auto ptr = std::make_shared<T>(std::move(value));
            std::lock_guard<std::mutex> lock(mMtx);
            mQueue.push(ptr);
            mCondVar.notify_one();
        }

        void Pop(T& value)
        {
            std::unique_lock<std::mutex> lock(mMtx);
            mCondVar.wait(lock, [this](){ return !mQueue.empty(); });
            value = std::move(*(mQueue.front()));
            mQueue.pop();
        }

        std::shared_ptr<T> Pop()
        {
            std::unique_lock<std::mutex> lock(mMtx);
            mCondVar.wait(lock, [this](){ return !mQueue.empty(); });
            auto res = mQueue.front();
            mQueue.pop();
            return res;
        }

        bool TryPop(T& value)
        {
            std::lock_guard<std::mutex> lock(mMtx);
            if (mQueue.empty())
            {
                return false;
            }
            value = std::move(*(mQueue.front()));
            mQueue.pop();
            return true;
        }

        std::shared_ptr<T> TryPop()
        {
            std::lock_guard<std::mutex> lock(mMtx);
            if (mQueue.empty())
            {
                return std::shared_ptr<T>();
            }
            auto res =  mQueue.front();
            mQueue.pop();
            return res;
        }

        std::shared_ptr<T> Front()
        {
            std::lock_guard<std::mutex> lock(mMtx);
            return mQueue.front();
        }

        std::shared_ptr<T> Back()
        {
            std::lock_guard<std::mutex> lock(mMtx);
            return mQueue.back();
        }

        bool IsEmpty() const
        {
            std::lock_guard<std::mutex> lock(mMtx);
            return mQueue.empty();
        }

    private:
        mutable std::mutex mMtx;
        std::queue<std::shared_ptr<T>> mQueue;
        std::condition_variable mCondVar;
    };
}
