#pragma once

#include <any>
#include <string>
#include <functional>

#include <typeindex>

namespace PlotGUI
{
	namespace Detail
	{
		class RegisterHelper;
	}

	class FieldMeta
	{
		friend class Detail::RegisterHelper;
	public:
		FieldMeta();

		template <typename Class, typename Field>
		FieldMeta(Field Class::* var);

	public:
		[[nodiscard]] const std::string& Name() const;
		[[nodiscard]] const std::type_index& Type() const;

		template <typename Field, typename Class>
		Field Get(const Class& c) const;

		//template <typename Field, typename Class>
		//Field Get(const Class* c) const;

		std::function<std::any(std::any)> GetFunc() const;

		template <typename Class, typename Field>
		void Set(Class& c, const Field& f) const;

		[[nodiscard]] const std::string& Attribute() const;

	private:
		std::string mName;
		std::type_index mType;
		std::function<std::any(std::any)> mGetFunc;
		std::function<void(std::any, std::any)> mSetFunc;

		std::string mAttribute;
	};

	template <typename Class, typename Field>
	FieldMeta::FieldMeta(Field Class::* var)
		: mType(typeid(var))
	{
		mGetFunc = [var](std::any c) -> std::any
		{
			return std::any_cast<const Class*>(c)->*var;
		};
		mSetFunc = [var](std::any c, std::any f)
		{
			std::any_cast<Class*>(c)->*var = std::any_cast<const Field&>(f);
		};
	}

	inline FieldMeta::FieldMeta()
		:mType(typeid(void))
	{
		
	}

	inline const std::string& FieldMeta::Name() const
	{
		return mName;
	}

	inline const std::type_index& FieldMeta::Type() const
	{
		return mType;
	}

	inline std::function<std::any(std::any)> FieldMeta::GetFunc() const
	{
		return mGetFunc;
	}

	inline const std::string& FieldMeta::Attribute() const
	{
		return mAttribute;
	}

	template <typename Field, typename Class>
	Field FieldMeta::Get(const Class& c) const
	{
		return std::any_cast<Field>(mGetFunc(&c));
	}

	//template <typename Field, typename Class>
	//Field FieldMeta::Get(const Class* c) const
	//{
	//	return std::any_cast<Field>(mGetFunc(c));
	//}

	template <typename Class, typename Field>
	void FieldMeta::Set(Class& c, const Field& f) const
	{
		mSetFunc(&c, f);
	}
}
