#pragma once

#include <type_traits>

template <typename T>
class Singleton
{
public:
    static T& GetInstance() noexcept(std::is_nothrow_constructible<T>::value)
    {
        static T sInstance;
        return sInstance;
    }
protected:
    Singleton() = default;
public:
    Singleton(const Singleton<T>& rhs) = delete;
    Singleton(Singleton<T>&& rhs) = delete;
    Singleton<T>& operator=(const Singleton<T>& rhs) = delete;
    Singleton<T>& operator=(Singleton<T>&& rhs) = delete;
    virtual ~Singleton() noexcept = default;
};
