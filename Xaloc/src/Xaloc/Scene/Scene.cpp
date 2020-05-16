#include "xapch.h"
#include "Scene.h"


namespace Xaloc {

	static const std::string DefaultGameObjectName = "New Game Object";

	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
		Init();
	}

	Scene::~Scene()
	{
		for (GameObject* gameObject : m_GameObjects)
			delete gameObject;
	}

	void Scene::Init()
	{

	}


	void Scene::OnUpdate(Timestep ts)
	{
		for (GameObject* gameObject : m_GameObjects)
		{
			gameObject->OnUpdate(ts);
		}
	}


	void Scene::AddGameObject(GameObject* gameObject)
	{
		m_GameObjects.push_back(gameObject);
	}

	GameObject* Scene::CreateGameObject(const std::string& name)
	{
		const std::string& goName = name.empty() ? DefaultGameObjectName : name;
		GameObject* go = new GameObject(goName);
		AddGameObject(go);
		return go;
	}
}