#pragma once

#include "PlotGUI/Utils/Singleton.h"
#include "ClassMeta.h"

#include <memory>
#include <unordered_map>
#include <map>

namespace PlotGUI
{
	namespace Detail
	{
		class RegisterHelper;
	}

	class Registry : public Singleton<Registry>
	{
	public:
		[[nodiscard]] const std::map<std::string, std::unique_ptr<ClassMeta>>& GetClassesMap() const;
		ClassMeta* Find(const std::string& className);
		void RegisterClassMeta(std::unique_ptr<ClassMeta> classMeta);
		void Clear();

	public:
		static Detail::RegisterHelper RegisterClassDe(const std::string& className, 
			const std::function<void* ()>& createFunc, const std::function<const void* (const void*)>& castFunc, 
			const std::string& attribute = std::string());

	private:
		std::map<std::string, std::unique_ptr<ClassMeta>> mClassMetas;
	};
}
