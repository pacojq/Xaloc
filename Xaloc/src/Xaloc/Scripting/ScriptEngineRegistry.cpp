#include "xapch.h"
#include "ScriptEngineRegistry.h"

#include <mono/jit/jit.h>

#include "Xaloc/Scene/Entity.h"

#include "Xaloc/Scene/Components/BehaviourComponent.h"
#include "Xaloc/Scene/Components/ColliderComponent.h"
#include "Xaloc/Scene/Components/IdComponent.h"
#include "Xaloc/Scene/Components/SceneComponent.h"
#include "Xaloc/Scene/Components/SpriteRendererComponent.h"
#include "Xaloc/Scene/Components/TransformComponent.h"
#include "Xaloc/Scene/Components/TagComponent.h"

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

	/// <summary>
	/// Register the C# component classes, including the methods
	/// Entity::HasComponent and Entity::AddComponent for each component
	/// type.
	/// </summary>
	static void InitComponentTypes()
	{
		Component_RegisterType(TagComponent);
		Component_RegisterType(TransformComponent);
		Component_RegisterType(SpriteRendererComponent);
	}

	void ScriptEngineRegistry::RegisterAll()
	{
		// Register C# component classes
		InitComponentTypes();

		
		// ============================ LOGGING ============================ //

		mono_add_internal_call("Xaloc.Log::Fatal_Native", Scripting::Xaloc_Log_Fatal);
		mono_add_internal_call("Xaloc.Log::Error_Native", Scripting::Xaloc_Log_Error);
		mono_add_internal_call("Xaloc.Log::Warn_Native", Scripting::Xaloc_Log_Warn);
		mono_add_internal_call("Xaloc.Log::Info_Native", Scripting::Xaloc_Log_Info);
		mono_add_internal_call("Xaloc.Log::Trace_Native", Scripting::Xaloc_Log_Trace);


		
		// ============================= INPUT ============================= //
		
		mono_add_internal_call("Xaloc.Input::IsKeyPressed_Native", Scripting::Xaloc_Input_IsKeyPressed);




		// ============================ ENTITIES ============================ //
		
		mono_add_internal_call("Xaloc.Entity::GetTransform_Native", Scripting::Xaloc_Entity_GetTransform);
		mono_add_internal_call("Xaloc.Entity::SetTransform_Native", Scripting::Xaloc_Entity_SetTransform);
		mono_add_internal_call("Xaloc.Entity::CreateComponent_Native", Scripting::Xaloc_Entity_CreateComponent);
		mono_add_internal_call("Xaloc.Entity::HasComponent_Native", Scripting::Xaloc_Entity_HasComponent);



		
		// =========================== COMPONENTS =========================== //
		
		mono_add_internal_call("Xaloc.TagComponent::GetTag_Native", Scripting::Xaloc_TagComponent_GetTag);
		mono_add_internal_call("Xaloc.TagComponent::SetTag_Native", Scripting::Xaloc_TagComponent_SetTag);
	}

}