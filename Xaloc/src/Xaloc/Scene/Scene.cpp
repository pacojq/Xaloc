#include "xapch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "SceneSerializer.h"

#include "Xaloc/Renderer/Renderer2D.h"

#include "Xaloc/Scripting/ScriptEngine.h"


#include <fstream>
#include <iomanip>
#include <iostream>

#include <pugixml.hpp>


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




	void Scene::Save(const Ref<Scene>& scene, const std::string& filename)
	{
		pugi::xml_document doc = SceneSerializer::Serialize(scene);
		bool success = doc.save_file(filename.c_str());
		
		if (!success)
		{
			XA_CORE_ERROR("Could not save scene in path '{}'", filename);
		}
		else XA_CORE_INFO("Scene saved in path '{}'", filename);
	}

	Ref<Scene> Scene::Load(const std::string& filename)
	{
		pugi::xml_document doc;
		
		std::ifstream stream(filename);
		pugi::xml_parse_result result = doc.load(stream);

		XA_CORE_TRACE("Scene read file result: {}", result.description());
		
		if (result.status != pugi::xml_parse_status::status_ok)
		{
			if (result.status == pugi::xml_parse_status::status_file_not_found)
			{
				XA_CORE_ASSERT(false, "Could not load scene. File not found.")
			}
			else
			{
				XA_CORE_ASSERT(false, "Could not load scene.")
			}
		}
		
		return SceneSerializer::Deserialize(doc);
	}


	
	
	Scene::Scene(const std::string& name)
		: m_Name(name), m_SceneID(s_SceneIDCounter++)
	{
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		m_Registry.on_construct<TagComponent>().connect<&OnTagConstruct>();
		m_Registry.on_construct<BehaviourComponent>().connect<&OnBehaviourComponentConstruct>();

		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID, m_Name);
		m_Registry.emplace<TagComponent>(m_SceneEntity, "Scene Entity");

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
		auto test = m_Registry.create();
		m_Registry.emplace<TagComponent>(test, "Test Collider Entity");
		m_Registry.emplace<TransformComponent>(test, glm::mat4(1.0f));
		m_Registry.emplace<ColliderComponent>(test, glm::vec2(1.0f));
	}


	void Scene::OnUpdate(Timestep ts)
	{
		// Update all entities
		{
			auto view = m_Registry.view<BehaviourComponent>();
			for (auto entity : view)
				ScriptEngine::OnUpdateEntity((uint32_t)entity, ts);
		}

		
		
		// For more info on entt groups:
		// https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/
		// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#groups

		// Physics
		{
			// Transform component will be owned by the Rendering group
			auto group = m_Registry.group<ColliderComponent>(entt::get<TransformComponent>);
			
			for (int i = 0; i < group.size(); i++)
			{
				auto entity = group[i];

				// Diagonal check
				for (int j = i+1; j < group.size(); j++)
				{
					auto other = group[j];

					// TODO check collision enter
				}
			}
			
			for (auto entity : group)
			{
				//auto [transformComponent, colliderComponent] = view.get<TransformComponent, ColliderComponent>(entity);
				//auto trans = transformComponent;
				//auto col = colliderComponent;

				// TODO post-process
			}
		}
		
		// Render all sprites
		//Renderer2D::BeginScene(*camera);
		{
			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
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
		entity.AddComponent<IdComponent>(m_NextEntityId);
		entity.AddComponent<TagComponent>(goName);
		
		m_NextEntityId++;
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity.m_EntityHandle);
	}


	void Scene::OnEvent(Event& e)
	{
		// TODO
		
		//auto view = m_Registry.view<CameraComponent>();
		//for (auto entity : view)
		//{
		//	auto& comp = view.get<CameraComponent>(entity);
		//	comp.Camera.OnEvent(e);
		//	break;
		//}
	}
}