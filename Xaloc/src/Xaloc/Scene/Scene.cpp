#include "xapch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "Xaloc/Renderer/Renderer2D.h"

#include "Xaloc/Scripting/ScriptEngine.h"

namespace Xaloc {

	static const std::string DefaultEntityName = "New Entity";

	std::unordered_map<uint32_t, Scene*> s_ActiveScenes;
	static uint32_t s_SceneIDCounter = 0;


	void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		XA_CORE_TRACE("New Transform component registered!");
	}

	void OnTagConstruct(entt::registry& registry, entt::entity entity)
	{

		auto& tag = registry.get<TagComponent>(entity);
		
		XA_CORE_TRACE("New Tag component registered: {0}", tag.Tag);
	}

	void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity entity)
	{
		XA_CORE_TRACE("New Behaviour component registered!");
		
		// Note: there should be exactly one scene component per registry
		auto view = registry.view<SceneComponent>();
		uint32_t sceneID = 0;
		for (auto entity : view)
		{
			auto& scene = registry.get<SceneComponent>(entity);
			sceneID = scene.SceneID;
		}

		ScriptEngine::OnInitEntity(registry.get<BehaviourComponent>(entity), (uint32_t)entity, sceneID);
	}

	Scene::Scene(const std::string& name)
		: m_Name(name), m_SceneID(s_SceneIDCounter++)
	{
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		m_Registry.on_construct<TagComponent>().connect<&OnTagConstruct>();
		m_Registry.on_construct<BehaviourComponent>().connect<&OnBehaviourComponentConstruct>();

		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);

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
		{
			auto view = m_Registry.view<BehaviourComponent>();
			for (auto entity : view)
				ScriptEngine::OnUpdateEntity((uint32_t)entity, ts);
		}

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



	Entity Scene::CreateEntity(const std::string& name)
	{
		const std::string& goName = name.empty() ? DefaultEntityName : name;
		
		auto entity = Entity{ m_Registry.create(), this };

		entity.AddComponent<TransformComponent>(glm::mat4(1.0f));
		entity.AddComponent<TagComponent>(goName);
		
		return entity;
	}
}