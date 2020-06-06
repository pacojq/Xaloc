#include "xapch.h"
#include "Scene.h"

#include "GameObject.h"

#include "Xaloc/Renderer/Renderer2D.h"

#include "Xaloc/Scene/Components/TagComponent.h"
#include "Xaloc/Scene/Components/TransformComponent.h"
#include "Xaloc/Scene/Components/SpriteRendererComponent.h"


namespace Xaloc {

	static const std::string DefaultGameObjectName = "New Game Object";

	std::unordered_map<uint32_t, Scene*> s_ActiveScenes;
	static uint32_t s_SceneIDCounter = 0;


	void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		XA_CORE_TRACE("Transform Component constructed!");
	}

	Scene::Scene(const std::string& name)
		: m_Name(name), m_SceneID(s_SceneIDCounter++)
	{
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		//m_Registry.on_construct<ScriptComponent>().connect<&OnScriptComponentConstruct>();

		m_SceneEntity = m_Registry.create();
		//m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);

		s_ActiveScenes[m_SceneID] = this;

		Init();
	}

	Scene::~Scene()
	{
		m_Registry.clear();
		s_ActiveScenes.erase(m_SceneID);
	}

	void Scene::Init()
	{

	}


	void Scene::OnUpdate(Timestep ts)
	{
		// Update all entities
		//{
		//	auto view = m_Registry.view<ScriptComponent>();
		//	for (auto entity : view)
		//		ScriptEngine::OnUpdateEntity((uint32_t)entity, ts);
		//}

		// Render all sprites
		//Renderer2D::BeginScene(*camera);
		{
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				auto trans = transformComponent;
				auto spr = spriteRendererComponent;
				Renderer2D::DrawQuad(transformComponent.Transform, spriteRendererComponent.SubTexture);
			}
		}

		//Renderer2D::EndScene();
	}



	GameObject Scene::CreateGameObject(const std::string& name)
	{
		const std::string& goName = name.empty() ? DefaultGameObjectName : name;
		
		auto go = GameObject{ m_Registry.create(), this };

		go.AddComponent<TransformComponent>(glm::mat4(1.0f));
		go.AddComponent<TagComponent>(goName);
		
		return go;
	}
}