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

} }