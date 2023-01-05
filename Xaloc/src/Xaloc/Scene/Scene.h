#pragma once

#include "CameraStack.h"
#include "PhysicSpace.h"

#include "Xaloc/Core/UUID.h"
#include "Xaloc/Core/Timestep.h"

#include "Xaloc/Events/Event.h"

#include "Xaloc/Renderer/Cameras/EditorCamera.h"

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

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, glm::mat4 cameraViewProj);
		
		void OnEvent(Event& e);

		const Ref<CameraStack>& GetCameraStack() const { return m_CameraStack; }

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
		std::string m_Name;
		entt::entity m_SceneEntity;

		Ref<CameraStack> m_CameraStack;

		entt::registry m_Registry;
		EntityMap m_EntityMap;

		Ref<PhysicSpace> m_PhysicSpace;

		
		friend class Entity;
		friend class SceneSerializer;
		friend class PhysicSpace;

		friend class SceneHierarchyPanel;
		
		friend void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity entity);
	};

}
