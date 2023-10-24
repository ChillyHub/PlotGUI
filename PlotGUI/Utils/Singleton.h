#pragma once

#include <type_traits>

namespace PlotGUI
{
	template<typename T>
	class Singleton
	{
	public:
		static T& Instance() noexcept(std::is_nothrow_constructible<T>::value)
		{
			static T ins;
			return ins;
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
}
