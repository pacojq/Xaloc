#pragma once

#include "Xaloc/Core/Timestep.h"

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


		Entity Scene::CreateEntity(const std::string& name);

	private:



	private:
		uint32_t m_SceneID;
		entt::entity m_SceneEntity;
		entt::registry m_Registry;

		std::string m_Name;

		
		friend class Entity;
	};

}