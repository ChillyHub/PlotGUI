#include <Reflection/Include/ClassMeta.h>

namespace PlotGUI
{
	const std::string& ClassMeta::GetClassNameRefl() const
	{
		return mName;
	}

	const std::vector<FieldMeta>& ClassMeta::GetFieldMetas() const
	{
		return mFieldMetas;
	}

	const std::vector<FuncMeta>& ClassMeta::GetFuncMetas() const
	{
		return mFuncMetas;
	}

	FieldMeta ClassMeta::GetFieldMeta(const std::string& fieldName) const
	{
		for (const auto & fieldMeta : mFieldMetas)
		{
			if (fieldMeta.Name() == fieldName)
			{
				return fieldMeta;
			}
		}

		throw std::runtime_error(std::string("Can't find field [") + mName + "::" + fieldName + "]");

		return FieldMeta{};
	}

	FuncMeta ClassMeta::GetFuncMeta(const std::string& funcName) const
	{
		for (const auto & funcMeta : mFuncMetas)
		{
			if (funcMeta.Name() == funcName)
			{
				return funcMeta;
			}
		}

		throw std::runtime_error(std::string("Can't find function [") + mName + "::" + funcName + "]");

		return FuncMeta{};
	}

	const std::string& ClassMeta::GetClassAttribute() const
	{
		return mAttribute;
	}

	void* ClassMeta::CreateClass() const
	{
		return mCreateFuncMetas();
	}

	const void* ClassMeta::CastClass(const void* src) const
	{
		return mCastFuncMetas(src);
	}
}
