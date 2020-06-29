#pragma once

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"

#include <entt/entt.hpp>

namespace Xaloc {

	class Entity;
	class SceneSerializer;
	
	class Scene
	{
	public:

		static void Save(const Ref<Scene>& scene, const std::string& filename);
		static Ref<Scene> Load(const std::string& filename);

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

		uint32_t m_NextEntityId = 0; // TODO move to universal ID
		
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}