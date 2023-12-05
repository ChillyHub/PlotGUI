#pragma once

#include "ArgWrap.h"

#include <any>
#include <tuple>
#include <string>
#include <functional>

namespace PlotGUI
{
	namespace Detail
	{
		class RegisterHelper;
	}

	class FuncMeta
	{
		friend class Detail::RegisterHelper;
	public:
		FuncMeta() = default;

		template <typename Return, typename Class, typename ...Args>
		explicit FuncMeta(Return (Class::* func)(Args...));

		template <typename Return, typename Class, typename ...Args>
		explicit FuncMeta(Return (Class::* func)(Args...) const);

		template <typename Class, typename ...Args>
		explicit FuncMeta(void (Class::* func)(Args...));

		template <typename Class, typename ...Args>
		explicit FuncMeta(void (Class::* func)(Args...) const);

	public:
		[[nodiscard]] const std::string& Name() const;
		[[nodiscard]] bool IsConst() const;

		template <typename Class, typename  ...Args>
		std::any Call(Class& c, Args&&... args);

		template <typename  ...Args>
		std::any Call(Args&&... args);

	private:
		std::string mName;
		bool mConst = false;
		int mArgsCount = 0;
		std::function<std::any(std::any)> mFunction;

		std::string mAttribute;
	};

	template <typename Return, typename Class, typename ... Args>
	FuncMeta::FuncMeta(Return(Class::* func)(Args...))
	{
		mArgsCount = sizeof...(Args);
		mFunction = [this, func](std::any args) -> std::any
		{
			auto& argsArray = *std::any_cast<std::array<Detail::ArgWrap, sizeof...(Args) + 1>*>(args);
			return std::apply(func, Detail::ArgsArrayToTuple<Class&, Args...>(argsArray));
		};
	}

	template <typename Return, typename Class, typename ... Args>
	FuncMeta::FuncMeta(Return(Class::* func)(Args...) const)
	{
		mConst = true;
		mArgsCount = sizeof...(Args);
		mFunction = [this, func](std::any args) -> std::any
		{
			auto& argsArray = *std::any_cast<std::array<Detail::ArgWrap, sizeof...(Args) + 1>*>(args);
			return std::apply(func, Detail::ArgsArrayToTuple<const Class&, Args...>(argsArray));
		};
	}

	template <typename Class, typename ... Args>
	FuncMeta::FuncMeta(void(Class::* func)(Args...))
	{
		mArgsCount = sizeof...(Args);
		mFunction = [this, func](std::any args) -> std::any
		{
			auto& argsArray = *std::any_cast<std::array<Detail::ArgWrap, sizeof...(Args) + 1>*>(args);
			std::apply(func, Detail::ArgsArrayToTuple<Class&, Args...>(argsArray));
			return std::any{};
		};
	}

	template <typename Class, typename ... Args>
	FuncMeta::FuncMeta(void(Class::* func)(Args...) const)
	{
		mConst = true;
		mArgsCount = sizeof...(Args);
		mFunction = [this, func](std::any args) -> std::any
		{
			auto& argsArray = *std::any_cast<std::array<Detail::ArgWrap, sizeof...(Args) + 1>*>(args);
			std::apply(func, Detail::ArgsArrayToTuple<const Class&, Args...>(argsArray));
			return std::any{};
		};
	}

	inline const std::string& FuncMeta::Name() const
	{
		return mName;
	}

	inline bool FuncMeta::IsConst() const
	{
		return mConst;
	}

	template <typename Class, typename ... Args>
	std::any FuncMeta::Call(Class& c, Args&&... args)
	{
		if (mArgsCount != sizeof...(Args))
		{
			throw std::runtime_error("Args count doesn't match");
		}
		std::array<Detail::ArgWrap, sizeof...(Args) + 1> wrap = {
			Detail::ArgWrap{std::forward<Class&>(c)}, Detail::ArgWrap{std::forward<Args>(args)}...
		};
		return mFunction(&wrap);
	}

	template <typename ... Args>
	std::any FuncMeta::Call(Args&&... args)
	{
		if (mArgsCount != sizeof...(Args))
		{
			throw std::runtime_error("Args count doesn't match");
		}
		std::array<Detail::ArgWrap, sizeof...(Args)> wrap = {
			Detail::ArgWrap{std::forward<Args>(args)}...
		};
		return mFunction(&wrap);
	}
}
