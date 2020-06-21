#pragma once

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"

#include <entt/entt.hpp>

namespace Xaloc {

	class Entity;

	struct SceneComponent
	{
		uint32_t SceneID;
	};
	
	class Scene
	{
	public:
		Scene(const std::string& name = "Scene");
		~Scene();

		void Init();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Entity Scene::CreateEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		template<typename T>
		auto FindEntitiesWith()
		{
			return m_Registry.view<T>();
		}

	private:



	private:
		uint32_t m_SceneID;
		entt::entity m_SceneEntity;
		entt::registry m_Registry;

		std::string m_Name;

		
		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}