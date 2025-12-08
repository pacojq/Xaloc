#include "xapch.h"
#include "xaloc/scripting/registry.h"
#include "xaloc/scripting/api.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/components.h"
#include <mono/jit/jit.h>


namespace Scripting
{

static std::unordered_map<MonoType*, ZFunctionHasComponent> s_HasComponentFuncs;
static std::unordered_map<MonoType*, ZFunctionCreateComponent> s_CreateComponentFuncs;

/// <summary>
/// Bind a C# component class to a C++ type, and register the methods:
///     - Entity::HasComponent
///     - Entity::AddComponent
/// </summary>
template<typename Component>
static void RegisterComponentType(MonoImage* pAssemblyImage, const std::string& sCSharpTypeName)
{
	const char* sType = sCSharpTypeName.c_str();
	MonoType* pType = mono_reflection_type_from_name((char*)sType, pAssemblyImage);
	const char* sTestName = mono_type_get_name_full(pType, MonoTypeNameFormat::MONO_TYPE_NAME_FORMAT_FULL_NAME);
	XA_CORE_WARN("Registering C# component type: {}", sTestName);
	if (pType)
	{
		uint32_t id = mono_type_get_type(pType);
		s_HasComponentFuncs[pType] = [](const Xaloc::ZEntity& entity) { return entity.HasComponent<Component>(); };
		s_CreateComponentFuncs[pType] = [](Xaloc::ZEntity& rEntity) { rEntity.AddComponent<Component>(); };
	}
	else
	{
		XA_CORE_ERROR("No C# component class found for {}!", typeid(Component).name());
	}
}

void RegisterEngineMethods(MonoImage* pAssemblyImage)
{
	// COMPONENTS
	RegisterComponentType<Xaloc::CmpTag>(pAssemblyImage, "Xaloc.TagComponent");
	RegisterComponentType<Xaloc::CmpTransform>(pAssemblyImage, "Xaloc.TransformComponent");
	RegisterComponentType<Xaloc::CmpSpriteRenderer>(pAssemblyImage, "Xaloc.SpriteRendererComponent");
	mono_add_internal_call("Xaloc.TagComponent::GetTag_Native", Scripting::Xaloc_TagComponent_GetTag);
	mono_add_internal_call("Xaloc.TagComponent::SetTag_Native", Scripting::Xaloc_TagComponent_SetTag);

	// ENTITIES
	mono_add_internal_call("Xaloc.Entity::GetTransform_Native", Scripting::Xaloc_Entity_GetTransform);
	mono_add_internal_call("Xaloc.Entity::SetTransform_Native", Scripting::Xaloc_Entity_SetTransform);
	mono_add_internal_call("Xaloc.Entity::CreateComponent_Native", Scripting::Xaloc_Entity_CreateComponent);
	mono_add_internal_call("Xaloc.Entity::HasComponent_Native", Scripting::Xaloc_Entity_HasComponent);

	// LOGGING
	mono_add_internal_call("Xaloc.Log::Fatal_Native", Scripting::Xaloc_Log_Fatal);
	mono_add_internal_call("Xaloc.Log::Error_Native", Scripting::Xaloc_Log_Error);
	mono_add_internal_call("Xaloc.Log::Warn_Native", Scripting::Xaloc_Log_Warn);
	mono_add_internal_call("Xaloc.Log::Info_Native", Scripting::Xaloc_Log_Info);
	mono_add_internal_call("Xaloc.Log::Trace_Native", Scripting::Xaloc_Log_Trace);

	// INPUT
	mono_add_internal_call("Xaloc.Input::IsKeyPressed_Native", Scripting::Xaloc_Input_IsKeyPressed);
}

ZFunctionHasComponent FindFunctionHasComponent(MonoType* pType)
{
	return s_HasComponentFuncs[pType];
}

ZFunctionCreateComponent FindFunctionCreateComponent(MonoType* pType)
{
	return s_CreateComponentFuncs[pType];
}

}