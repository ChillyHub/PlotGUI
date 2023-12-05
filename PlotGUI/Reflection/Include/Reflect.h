#pragma once

#include "Registry.h"
#include "RegisterHelper.h"

#include "Component/Script.h"

namespace PlotGUI
{
	template <typename Class>
	class Registrar
	{
	public:
		Registrar()
		{
			auto createFunc = []() -> Class*
				{
					Class* object = new Class();
					return object;
				};
			auto castFunc = [](const void* src) -> const Class*
				{
					return static_cast<const Class*>(src);
				};
			Class::Register(createFunc, castFunc);
		}
	};
}

#define RCLASS class

#define RCLASS_END(classTypeName) static PlotGUI::Registrar<classTypeName> sRegistrar##classTypeName;

#define REGISTER_FUNC(createFunc, castFunc) static void Register(const std::function<void*()>& createFunc, const std::function<const void*(const void*)>& castFunc)

#define REFLECT_CREATE(classTypeReturn, classTypeName) (classTypeReturn)PlotGUI::Registry::Instance().Find(classTypeName)->CreateClass()

#define REFLECT_GET(className, classTypeName, fieldName, fieldTypeName) \
	PlotGUI::Registry::Instance().Find(classTypeName)->GetFieldMeta(fieldName).Get<fieldTypeName>(className)

#define REFLECT_SET(className, classTypeName, fieldName, fieldValue) \
	PlotGUI::Registry::Instance().Find(classTypeName)->GetFieldMeta(fieldName).Set(className, fieldValue)

#define REFLECT_CALL(className, classTypeName, funcName, ...) \
	PlotGUI::Registry::Instance().Find(classTypeName)->GetFuncMeta(funcName).Call(className, ##__VA_ARGS__)

#define REFLECT_CALL_RETURN(returnType, className, classTypeName, funcName, ...) \
	std::any_cast<#returnType>(PlotGUI::Registry::Instance().Find(classTypeName)->GetFuncMeta(funcName).Call(className, ##__VA_ARGS__))

#define REFLECT_ATTRIBUTE(className, classTypeName, fieldName, ...) \
	PlotGUI::Registry::Instance().Find(classTypeName)->GetFieldMeta(fieldName).Attribute()
