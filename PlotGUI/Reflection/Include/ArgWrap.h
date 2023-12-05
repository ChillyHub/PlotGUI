#pragma once

#include <any>
#include <array>
#include <tuple>
#include <stdexcept>
#include <type_traits>

namespace PlotGUI::Detail
{
	enum class Ref
	{
		NoneRef = 0,
		LValRef,
		RValRef
	};

	template <typename T>
	struct RefType
	{
		static constexpr Ref value = Ref::NoneRef;
	};

	template <typename T>
	struct RefType<T&>
	{
		static constexpr Ref value = Ref::LValRef;
	};

	template <typename T>
	struct RefType<T&&>
	{
		static constexpr Ref value = Ref::RValRef;
	};

	template <typename T>
	struct IsConst : std::false_type {};

	template <typename T>
	struct IsConst<const T> : std::true_type {};

	template <typename T>
	struct IsConst<const T&> : std::true_type {};

	template <typename T>
	struct IsConst<const T&&> : std::true_type {};

	template <typename T>
	struct IsConst<const T*> : std::true_type {};

	class ArgWrap
	{
	public:
		template <typename T>
		ArgWrap(T&& arg);

		template <typename T>
		T ArgCast();

	private:
		std::any mData;
		Ref mRefType = Ref::NoneRef;
		bool mIsConst = false;
	};

	template <typename T>
	ArgWrap::ArgWrap(T&& arg)
	{
		mRefType = RefType<T>::value;
		mIsConst = IsConst<T>::value;

		if (mRefType == Ref::LValRef)
		{
			mData = &arg;
		}
		else if (mRefType == Ref::RValRef)
		{
			throw std::runtime_error("Unsupported r-value reference");
		}
		else
		{
			mData = arg;
		}
	}

	template <typename T>
	T ArgWrap::ArgCast()
	{
		using BaseT = std::remove_cv_t<std::remove_reference_t<T>>;
		constexpr Ref refType = RefType<T>::value;
		constexpr bool isConst = IsConst<T>::value;

		if constexpr (refType == Ref::NoneRef)
		{
			if (mRefType == Ref::NoneRef)
			{
				return std::any_cast<BaseT>(mData);
			}
			else if (mRefType == Ref::LValRef)
			{
				if (mIsConst)
				{
					return *std::any_cast<const BaseT*>(mData);
				}
				else
				{
					return *std::any_cast<BaseT*>(mData);
				}
			}
			else
			{
				throw std::runtime_error("Unsupported r-value reference");
			}
		}
		else if constexpr (refType == Ref::LValRef)
		{
			if (mRefType == Ref::NoneRef)
			{
				return *std::any_cast<BaseT>(&mData);
			}
			else if (mRefType == Ref::LValRef)
			{
				if constexpr (isConst)
				{
					if (mIsConst)
					{
						return *std::any_cast<const BaseT*>(mData);
					}
					else
					{
						return *std::any_cast<BaseT*>(mData);
					}
				}
				else
				{
					if (mIsConst)
					{
						throw std::runtime_error("Can not cast const reference to non-const reference");
					}
					else
					{
						return *std::any_cast<BaseT*>(mData);
					}
				}
			}
			else
			{
				throw std::runtime_error("Unsupported r-value reference");
			}
		}
		else
		{
			throw std::runtime_error("Unsupported r-value reference");
		}
	}

	template <typename... Args, size_t N, size_t... Index>
	std::tuple<Args...> ArgsArrayToTuple(std::array<ArgWrap, N>& args, std::index_sequence<Index...>)
	{
		return std::forward_as_tuple(args[Index].template ArgCast<Args>()...);
	}

	template <typename... Args, size_t N>
	//requires (sizeof...(Args) == N)
	std::tuple<Args...> ArgsArrayToTuple(std::array<ArgWrap, N>& args)
	{
		return ArgsArrayToTuple<Args...>(args, std::make_index_sequence<N>{});
	}
}
