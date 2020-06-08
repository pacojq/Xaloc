#include "xapch.h"
#include "ScriptEngineRegistry.h"

#include <mono/jit/jit.h>

#include "Xaloc/Scene/Components.h"
#include "Xaloc/Scene/Entity.h"

#include "ScriptAPI.h"


namespace Xaloc {

	std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;

	extern MonoImage* s_CoreAssemblyImage;

	#define Component_RegisterType(Type) \
	{\
		MonoType* type = mono_reflection_type_from_name("Xaloc." #Type, s_CoreAssemblyImage);\
		if (type) {\
			uint32_t id = mono_type_get_type(type);\
			s_HasComponentFuncs[type] = [](Entity& entity) { return entity.HasComponent<Type>(); };\
			s_CreateComponentFuncs[type] = [](Entity& entity) { entity.AddComponent<Type>(); };\
		} else {\
			XA_CORE_ERROR("No C# component class found for " #Type "!");\
		}\
	}

	static void InitComponentTypes()
	{
		Component_RegisterType(TagComponent);
		Component_RegisterType(TransformComponent);
		Component_RegisterType(SpriteRendererComponent);
	}

	void ScriptEngineRegistry::RegisterAll()
	{
		InitComponentTypes();

		mono_add_internal_call("Xaloc.Input::IsKeyPressed_Native", Xaloc::Scripting::Xaloc_Input_IsKeyPressed);

		mono_add_internal_call("Xaloc.Entity::GetTransform_Native", Xaloc::Scripting::Xaloc_Entity_GetTransform);
		mono_add_internal_call("Xaloc.Entity::SetTransform_Native", Xaloc::Scripting::Xaloc_Entity_SetTransform);
		mono_add_internal_call("Xaloc.Entity::CreateComponent_Native", Xaloc::Scripting::Xaloc_Entity_CreateComponent);
		mono_add_internal_call("Xaloc.Entity::HasComponent_Native", Xaloc::Scripting::Xaloc_Entity_HasComponent);
	}

}