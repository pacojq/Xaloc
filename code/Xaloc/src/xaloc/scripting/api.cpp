#include "xapch.h"
#include "xaloc/scripting/api.h"
#include "xaloc/scripting/registry.h"
#include "xaloc/scene/scene.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/components.h"
#include "xaloc/core/Input/Input.h"
#include "xaloc/debugging/assert.h"
#include <glm/gtc/type_ptr.hpp>
#include <mono/jit/jit.h>


namespace Scripting
{

static std::string FromMonoString(MonoString* str)
{
	mono_unichar2* chl = mono_string_chars(str);
	std::string out("");
	for (int i = 0; i < mono_string_length(str); i++)
		out += chl[i];
	return out;
}

static Xaloc::ZEntity FindEntity(uint64_t id)
{
	auto scene = ZScriptEngine::GetCurrentSceneContext();
	XA_CORE_ASSERT(scene, "No active scene!");
	const Xaloc::EntityMap& entityMap = scene->GetEntityMap();
	XA_CORE_ASSERT(entityMap.find(id) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

	return entityMap.at(id);
}

// ============================ LOGGING ============================ //
	
void Xaloc_Log_Fatal(MonoString* msg) { XA_FATAL(FromMonoString(msg)); }
void Xaloc_Log_Error(MonoString* msg) { XA_ERROR(FromMonoString(msg)); }
void Xaloc_Log_Warn(MonoString* msg) { XA_WARN(FromMonoString(msg)); }
void Xaloc_Log_Info(MonoString* msg) { XA_INFO(FromMonoString(msg)); }
void Xaloc_Log_Trace(MonoString* msg) { XA_TRACE(FromMonoString(msg)); }

// ============================= INPUT ============================= //
	
bool Xaloc_Input_IsKeyPressed(Xaloc::EKeyCode key) { return Xaloc::ZInput::IsKeyPressed(key); }

// ============================ ENTITIES ============================ //

void Xaloc_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform)
{
	Xaloc::ZEntity entity = FindEntity(entityID);
	auto& transformComponent = entity.GetComponent<Xaloc::CmpTransform>();
	memcpy(outTransform, glm::value_ptr(transformComponent.GetTransform()), sizeof(glm::mat4));
}

void Xaloc_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform)
{
	Xaloc::ZEntity entity = FindEntity(entityID);

	auto& transformComponent = entity.GetComponent<Xaloc::CmpTransform>();

	glm::vec3 scale, translation, skew;
	glm::vec4 perspective;
	glm::quat orientation;
	glm::decompose(*inTransform, scale, orientation, translation, skew, perspective);

	if (glm::abs(orientation.y < 0.001f))
	{
		orientation.y = 0.0f;
	}

	transformComponent.Translation = translation;
	transformComponent.Rotation = glm::degrees(glm::eulerAngles(orientation));
	transformComponent.Scale = scale;
}

void Xaloc_Entity_CreateComponent(uint64_t entityID, void* type)
{
	Xaloc::ZEntity entity = FindEntity(entityID);
	MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
	const ZFunctionHasComponent function = FindFunctionHasComponent(monoType);
	function(entity);
}

bool Xaloc_Entity_HasComponent(uint64_t entityID, void* type)
{
	const Xaloc::ZEntity entity = FindEntity(entityID);
	MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
	const ZFunctionHasComponent function = FindFunctionHasComponent(monoType);
	const bool bResult = function(entity);
	return bResult;
}

// =========================== COMPONENTS =========================== //

MonoString* Xaloc_TagComponent_GetTag(uint64_t entityID)
{
	Xaloc::ZEntity entity = FindEntity(entityID);
	auto& tagComponent = entity.GetComponent<Xaloc::CmpTag>();

	std::string tag = tagComponent.sTag;
		
	return mono_string_new(mono_domain_get(), tag.c_str());
}
	
void Xaloc_TagComponent_SetTag(uint64_t entityID, MonoString* pTag)
{
	const Xaloc::ZEntity entity = FindEntity(entityID);
	Xaloc::CmpTag& rTag = entity.GetComponent<Xaloc::CmpTag>();

	const std::string sTag = FromMonoString(pTag);
	rTag.sTag = sTag;
}

}