#include <Reflection/Include/RegisterHelper.h>

namespace PlotGUI::Detail
{
	RegisterHelper::RegisterHelper(const std::string& className, 
		const std::function<void* ()>& createFunc, const std::function<const void* (const void*)>& castFunc, 
		const std::string& attribute)
		: mClassMeta(std::make_unique<ClassMeta>())
	{
		mClassMeta->mName = className;
		mClassMeta->mCreateFuncMetas = createFunc;
		mClassMeta->mCastFuncMetas = castFunc;
		mClassMeta->mAttribute = attribute;
	}

	RegisterHelper::~RegisterHelper()
	{
		Registry::Instance().RegisterClassMeta(std::move(mClassMeta));
	}
}
