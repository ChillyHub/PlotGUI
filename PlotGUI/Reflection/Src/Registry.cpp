#include "Reflection/Include/Registry.h"
#include "Reflection/Include/RegisterHelper.h"

namespace PlotGUI
{
	const std::map<std::string, std::unique_ptr<ClassMeta>>& Registry::GetClassesMap() const
	{
		return mClassMetas;
	}

	ClassMeta* Registry::Find(const std::string& className)
	{
		if (mClassMetas.find(className) == mClassMetas.end())
		{
			throw std::runtime_error(std::string("Can't find Class [") + className + "]");
		}

		return mClassMetas[className].get();
	}

	void Registry::RegisterClassMeta(std::unique_ptr<ClassMeta> classMeta)
	{
		mClassMetas[classMeta->GetClassNameRefl()] = std::move(classMeta);
	}

	void Registry::Clear()
	{
		decltype(mClassMetas) tmp;
		tmp.swap(mClassMetas);
	}

	Detail::RegisterHelper Registry::RegisterClassDe(const std::string& className, 
		const std::function<void*()>& createFunc, const std::function<const void* (const void*)>& castFunc, const std::string& attribute)
	{
		return Detail::RegisterHelper(className, createFunc, castFunc, attribute);
	}
}
