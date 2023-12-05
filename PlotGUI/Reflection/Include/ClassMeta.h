#pragma once

#include "FieldMeta.h"
#include "FuncMeta.h"

#include <vector>

namespace PlotGUI
{
	namespace Detail
	{
		class RegisterHelper;
	}

	class ClassMeta
	{
		friend class Detail::RegisterHelper;
	public:
		[[nodiscard]] const std::string& GetClassNameRefl() const;
		[[nodiscard]] const std::vector<FieldMeta>& GetFieldMetas() const;
		[[nodiscard]] const std::vector<FuncMeta>& GetFuncMetas() const;
		[[nodiscard]] FieldMeta GetFieldMeta(const std::string& fieldName) const;
		[[nodiscard]] FuncMeta GetFuncMeta(const std::string& funcName) const;

		[[nodiscard]] const std::string& GetClassAttribute() const;

		[[nodiscard]] void* CreateClass() const;
		const void* CastClass(const void* src) const;

	private:
		std::string mName;
		std::vector<FieldMeta> mFieldMetas;
		std::vector<FuncMeta> mFuncMetas;

		std::function<void*()> mCreateFuncMetas;
		std::function<const void* (const void*)> mCastFuncMetas;

		std::string mAttribute;
	};
}
