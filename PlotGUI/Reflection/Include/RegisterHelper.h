#pragma once

#include "Registry.h"

namespace PlotGUI::Detail
{
	class RegisterHelper
	{
	public:
		explicit RegisterHelper(const std::string& className, 
			const std::function<void* ()>& createFunc, const std::function<const void*(const void*)>& castFunc, const std::string& attribute);
		~RegisterHelper();

		RegisterHelper(const RegisterHelper& rhs) = delete;
		RegisterHelper& operator=(const RegisterHelper& rhs) = delete;

		RegisterHelper(RegisterHelper&& rhs) = default;
		RegisterHelper& operator=(RegisterHelper&& rhs) = default;

	public:
		template <typename Class, typename Field>
		RegisterHelper& RegisterField(const std::string& fieldName, Field Class::* field, const std::string& attribute = std::string());

		template <typename Func>
		RegisterHelper& RegisterFunc(const std::string& funcName, Func func, const std::string& attribute = std::string());

	private:
		std::unique_ptr<ClassMeta> mClassMeta;
	};

	template <typename Class, typename Field>
	RegisterHelper& RegisterHelper::RegisterField(const std::string& fieldName, Field Class::* field, const std::string& attribute)
	{
		FieldMeta fm(field);
		fm.mName = fieldName;
		fm.mAttribute = attribute;
		mClassMeta->mFieldMetas.push_back(std::move(fm));
		return *this;
	}

	template <typename Func>
	RegisterHelper& RegisterHelper::RegisterFunc(const std::string& funcName, Func func, const std::string& attribute)
	{
		FuncMeta fm(func);
		fm.mName = funcName;
		fm.mAttribute = attribute;
		mClassMeta->mFuncMetas.push_back(std::move(fm));
		return *this;
	}
}
