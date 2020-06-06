#pragma once

#include "Xaloc/Core/Timestep.h"

#include <entt/entt.hpp>

namespace Xaloc {

	class GameObject;

	class Scene
	{
	public:
		Scene(const std::string& name = "Scene");
		~Scene();

		void Init();


		void OnUpdate(Timestep ts);


		GameObject Scene::CreateGameObject(const std::string& name);

	private:



	private:
		uint32_t m_SceneID;
		entt::entity m_SceneEntity;
		entt::registry m_Registry;

		std::string m_Name;

		
		friend class GameObject;
	};

}