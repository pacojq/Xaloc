#include "xapch.h"
#include "ScriptAPI.h"

#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Entity.h"
#include "Xaloc/Scene/Components.h"

#include "Xaloc/Core/Input/Input.h"

#include <glm/gtc/type_ptr.hpp>

#include <mono/jit/jit.h>

namespace Xaloc {

	// Maps used to store entity components
	extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}




namespace Xaloc { namespace Scripting {

	static std::string FromMonoString(MonoString* str)
	{
		mono_unichar2* chl = mono_string_chars(str);
		std::string out("");
		for (int i = 0; i < mono_string_length(str); i++)
			out += chl[i];
		return out;
	}

	static Entity FindEntity(uint64_t id)
	{
		auto scene = ScriptEngine::GetCurrentSceneContext();
		XA_CORE_ASSERT(scene, "No active scene!");
		auto entityMap = scene->GetEntityMap();
		XA_CORE_ASSERT(entityMap.find(id) != entityMap.end(), "Invalid entity ID or entity doesn't exist in scene!");

		return entityMap[id];
	}
	
	

	// ============================ LOGGING ============================ //
	
	void Xaloc_Log_Fatal(MonoString* msg) { XA_FATAL(FromMonoString(msg)); }
	void Xaloc_Log_Error(MonoString* msg) { XA_ERROR(FromMonoString(msg)); }
	void Xaloc_Log_Warn(MonoString* msg) { XA_WARN(FromMonoString(msg)); }
	void Xaloc_Log_Info(MonoString* msg) { XA_INFO(FromMonoString(msg)); }
	void Xaloc_Log_Trace(MonoString* msg) { XA_TRACE(FromMonoString(msg)); }



	

	// ============================= INPUT ============================= //
	
	bool Xaloc_Input_IsKeyPressed(KeyCode key) { return Input::IsKeyPressed(key); }




	// ============================ ENTITIES ============================ //

	void Xaloc_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform)
	{
		Entity entity = FindEntity(entityID);
		auto& transformComponent = entity.GetComponent<TransformComponent>();
		memcpy(outTransform, glm::value_ptr(transformComponent.GetTransform()), sizeof(glm::mat4));
	}

	void Xaloc_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform)
	{
		Entity entity = FindEntity(entityID);

		auto& transformComponent = entity.GetComponent<TransformComponent>();

		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;
		glm::decompose(*inTransform, scale, orientation, translation, skew, perspective);

		if (glm::abs(orientation.y < 0.001f))
			orientation.y = 0.0f;

		transformComponent.Translation = translation;
		transformComponent.Rotation = glm::degrees(glm::eulerAngles(orientation));
		transformComponent.Scale = scale;
	}

	void Xaloc_Entity_CreateComponent(uint64_t entityID, void* type)
	{
		Entity entity = FindEntity(entityID);
		MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
		s_CreateComponentFuncs[monoType](entity);
	}

	bool Xaloc_Entity_HasComponent(uint64_t entityID, void* type)
	{
		Entity entity = FindEntity(entityID);
		MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
		bool result = s_HasComponentFuncs[monoType](entity);
		return result;
	}





	// =========================== COMPONENTS =========================== //

	MonoString* Xaloc_TagComponent_GetTag(uint64_t entityID)
	{
		Entity entity = FindEntity(entityID);
		auto& tagComponent = entity.GetComponent<TagComponent>();

		std::string tag = tagComponent.Tag;
		
		return mono_string_new(mono_domain_get(), tag.c_str());
	}
	
	void Xaloc_TagComponent_SetTag(uint64_t entityID, MonoString* inTag)
	{
		Entity entity = FindEntity(entityID);
		auto& tagComponent = entity.GetComponent<TagComponent>();

		std::string tag = FromMonoString(inTag);
		tagComponent.Tag = tag;
	}




	
} }