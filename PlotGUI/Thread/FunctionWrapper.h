#pragma once

#include <memory>

namespace PlotGUI
{
    class FunctionWrapper
    {
        struct FuncBase
        {
            virtual ~FuncBase() = default;

            virtual void Call() = 0;
        };

        template <typename T>
        struct FuncType : public FuncBase
        {
            T func;

            explicit FuncType(T&& f) : func(std::move(f)) {}

            virtual void Call() override
            {
                func();
            }
        };      
        
    public:
        FunctionWrapper() = default;

        template <typename T>
        FunctionWrapper(T&& func) : mFunction(new FuncType<T>(std::forward<T>(func))) {}

        FunctionWrapper(const FunctionWrapper& rhs) = delete;
        FunctionWrapper& operator=(const FunctionWrapper& rhs) = delete;

        FunctionWrapper(FunctionWrapper&& rhs) noexcept : mFunction(std::move(rhs.mFunction)) {}
        FunctionWrapper& operator=(FunctionWrapper&& rhs) noexcept
        {
            mFunction = std::move(rhs.mFunction);
            return *this;
        }

        void operator()()
        {
            mFunction->Call();
        }

    private:
        std::unique_ptr<FuncBase> mFunction;
    };
}
