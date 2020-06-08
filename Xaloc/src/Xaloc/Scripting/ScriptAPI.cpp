#include "xapch.h"
#include "ScriptAPI.h"

#include "Xaloc/Scene/Entity.h"
#include "Xaloc/Core/Input/Input.h"

#include <mono/jit/jit.h>

namespace Xaloc {
	extern std::unordered_map<uint32_t, Scene*> s_ActiveScenes;
	extern std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	extern std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;
}




namespace Xaloc { namespace Scripting {


	// = = = = = = = = = = = = = INPUT = = = = = = = = = = = = = //

	std::string FromMonoString(MonoString* str)
	{
		mono_unichar2* chl = mono_string_chars(str);
		std::string out("");
		for (int i = 0; i < mono_string_length(str); i++)
			out += chl[i];
		return out;
	}
	
	void Xaloc_Log_Fatal(MonoString* msg) { XA_FATAL(FromMonoString(msg)); }
	void Xaloc_Log_Error(MonoString* msg) { XA_ERROR(FromMonoString(msg)); }
	void Xaloc_Log_Warn(MonoString* msg) { XA_WARN(FromMonoString(msg)); }
	void Xaloc_Log_Info(MonoString* msg) { XA_INFO(FromMonoString(msg)); }
	void Xaloc_Log_Trace(MonoString* msg) { XA_TRACE(FromMonoString(msg)); }



	
	
	// = = = = = = = = = = = = = INPUT = = = = = = = = = = = = = //
	
	bool Xaloc_Input_IsKeyPressed(KeyCode key)
	{
		return Input::IsKeyPressed(key);
	}




	// = = = = = = = = = = = = = ENTITY = = = = = = = = = = = = = //

	void Xaloc_Entity_GetTransform(uint32_t sceneID, uint32_t entityID, glm::mat4* outTransform)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		auto& transformComponent = entity.GetComponent<TransformComponent>();
		memcpy(outTransform, glm::value_ptr(transformComponent.Transform), sizeof(glm::mat4));
	}

	void Xaloc_Entity_SetTransform(uint32_t sceneID, uint32_t entityID, glm::mat4* inTransform)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		auto& transformComponent = entity.GetComponent<TransformComponent>();
		memcpy(glm::value_ptr(transformComponent.Transform), inTransform, sizeof(glm::mat4));
	}

	void Xaloc_Entity_CreateComponent(uint32_t sceneID, uint32_t entityID, void* type)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");
		MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		s_CreateComponentFuncs[monoType](entity);
	}

	bool Xaloc_Entity_HasComponent(uint32_t sceneID, uint32_t entityID, void* type)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");
		MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		bool result = s_HasComponentFuncs[monoType](entity);
		return result;
	}





	// = = = = = = = = = = = = = TAG COMPONENT = = = = = = = = = = = = = //

	MonoString* Xaloc_TagComponent_GetTag(uint32_t sceneID, uint32_t entityID)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		auto& tagComponent = entity.GetComponent<TagComponent>();

		std::string tag = tagComponent.Tag;
		
		return mono_string_new(mono_domain_get(), tag.c_str());
	}
	
	void Xaloc_TagComponent_SetTag(uint32_t sceneID, uint32_t entityID, MonoString* inTag)
	{
		XA_CORE_ASSERT(s_ActiveScenes.find(sceneID) != s_ActiveScenes.end(), "Invalid Scene ID!");

		Scene* scene = s_ActiveScenes[sceneID];
		Entity entity((entt::entity)entityID, scene);
		auto& tagComponent = entity.GetComponent<TagComponent>();

		std::string tag = FromMonoString(inTag);
		tagComponent.Tag = tag;
	}




	
} }