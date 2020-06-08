#pragma once

#include "MonoUtils.h"

#include "Xaloc/Scene/Scene.h"

#include <mono/jit/jit.h>
#include <mono/metadata/debug-helpers.h>


extern "C" {
	typedef struct _MonoObject MonoObject;
}

namespace Xaloc {

	
	
	struct EntityBehaviourClass
	{
		std::string FullName;
		std::string ClassName;
		std::string NamespaceName;

		MonoClass* Class;
		MonoMethod* MethodOnCreate;
		MonoMethod* MethodOnDestroy;
		MonoMethod* MethodOnUpdate;

		void InitClassMethods(MonoImage* image)
		{
			MethodOnCreate = MonoUtils::GetMethod(image, FullName + ":OnCreate()");
			MethodOnUpdate = MonoUtils::GetMethod(image, FullName + ":OnUpdate(single)");
		}
	};
	
	struct EntityInstance
	{
		EntityBehaviourClass* ScriptClass;

		uint32_t Handle;
		Scene* SceneInstance;

		MonoObject* GetInstance()
		{
			return mono_gchandle_get_target(Handle);
		}
	};

}