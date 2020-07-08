#pragma once

#include "UUID.h"
#include "PhysicSpace.h"

#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"

#include <entt/entt.hpp>

namespace Xaloc {

	class Entity;
	using EntityMap = std::unordered_map<UUID, Entity>;
	
	class SceneSerializer;
	
	class Scene
	{
	public:

		static void Save(const Ref<Scene>& scene, const std::string& filename);
		static Ref<Scene> Load(const std::string& filename);

		Scene(const std::string& name = "Scene");
		Scene(const std::string& name, UUID id);
		~Scene();

		void Init();

		void StartRuntime();

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Entity Scene::CreateEntity(const std::string& name);
		Entity Scene::CreateEntity(const std::string& name, UUID id);
		void DestroyEntity(Entity entity);

		template<typename T>
		auto FindEntitiesWith()
		{
			return m_Registry.view<T>();
		}

		UUID const GetID() const { return m_SceneID; }
		const EntityMap& const GetEntityMap() const { return m_EntityMap; }

	private:
		UUID m_SceneID;
		entt::entity m_SceneEntity;
		entt::registry m_Registry;

		EntityMap m_EntityMap;

		std::string m_Name;

		Ref<PhysicSpace> m_PhysicSpace;



		
		friend class Entity;
		friend class SceneSerializer;
		friend class PhysicSpace;

		friend class SceneHierarchyPanel;
		
		friend void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity entity);
	};

}