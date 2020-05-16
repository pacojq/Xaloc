#pragma once

#include "GameObject.h"

#include "Xaloc/Core/Timestep.h"


namespace Xaloc {

	class Scene
	{
	public:
		Scene(const std::string& name = "Scene");
		~Scene();

		void Init();


		void OnUpdate(Timestep ts);

		void AddGameObject(GameObject* gameObject);
		GameObject* Scene::CreateGameObject(const std::string& name);

	private:



	private:
		std::string m_Name;
		std::vector<GameObject*> m_GameObjects;

		// TODO friend class???

	};

}