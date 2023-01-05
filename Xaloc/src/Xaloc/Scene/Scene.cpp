#include "xapch.h"
#include "Scene.h"

#include "Components.h"
#include "Entity.h"

#include "SceneSerializer.h"

#include "Xaloc/Core/Assets/AssetManager.h"

#include "Xaloc/Renderer/Renderer2D.h"
#include "Xaloc/Renderer/Camera.h"

#include "Xaloc/Scripting/ScriptEngine.h"


#include <fstream>
#include <iomanip>
#include <iostream>

#include <limits>



namespace Xaloc {

	static const std::string DefaultEntityName = "New Entity";

	std::unordered_map<UUID, Scene*> s_ActiveScenes;


	void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
		XA_CORE_TRACE("New Transform component registered!");
	}

	void OnTagConstruct(entt::registry& registry, entt::entity entity)
	{

		auto& tag = registry.get<TagComponent>(entity);
		
		XA_CORE_TRACE("New Tag component registered: {0}", tag.Tag);
	}

	void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity e)
	{
		XA_CORE_TRACE("");
		
		auto view = registry.view<SceneComponent>();
		UUID sceneID = registry.get<SceneComponent>(view.front()).SceneID;
		Scene* scene = s_ActiveScenes[sceneID];
		
		auto entityID = registry.get<IdComponent>(e).ID;
		XA_CORE_ASSERT(scene->m_EntityMap.find(entityID) != scene->m_EntityMap.end(), "Could not find Behaviour entity!");

		ScriptEngine::InitBehaviourEntity(scene->m_EntityMap.at(entityID));
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
				XA_CORE_ASSERT(false, "Could not load scene. File not found.");
			}
			else
			{
				XA_CORE_ASSERT(false, "Could not load scene.");
			}
		}
		
		return SceneSerializer::Deserialize(doc);
	}


	
	
	Scene::Scene(const std::string& name)
		: m_Name(name)
	{
		Init();
	}

	Scene::Scene(const std::string& name, UUID id)
		: m_Name(name), m_SceneID(id)
	{
		Init();
	}
	

	Scene::~Scene()
	{
		m_Registry.clear();
		s_ActiveScenes.erase(m_SceneID);
	}

	void Scene::Init()
	{
		m_CameraStack = CreateRef<CameraStack>();

		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		m_Registry.on_construct<TagComponent>().connect<&OnTagConstruct>();
		m_Registry.on_construct<BehaviourComponent>().connect<&OnBehaviourComponentConstruct>();

		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID, m_Name);
		m_Registry.emplace<TagComponent>(m_SceneEntity, "Scene Entity");

		s_ActiveScenes[m_SceneID] = this;
		
		
		auto test = m_Registry.create();
		m_Registry.emplace<TagComponent>(test, "Test Collider Entity");
		m_Registry.emplace<TransformComponent>(test);
		m_Registry.emplace<ColliderComponent>(test, glm::vec2(1.0f));


	}

	void Scene::StartRuntime()
	{
		// TODO we should know if we are on runtime or not, since ScriptEngine::InstantiateEntityClass will need to be called on behaviors created AFTER this point
		
		XA_CORE_INFO("Starting Script Runtime...");
		
		auto view = m_Registry.view<BehaviourComponent>();
		for (auto entity : view)
		{
			Entity e = { entity, this };
			if (ScriptEngine::ModuleExists(e.GetComponent<BehaviourComponent>().ModuleName))
				ScriptEngine::InstantiateEntityClass(e);
			else XA_CORE_WARN("Could not initialize behaviour '{0}'. Module does not exist.", e.GetComponent<BehaviourComponent>().ModuleName);
		}
	}

	

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update all entities
		{
			auto view = m_Registry.view<BehaviourComponent>();
			for (auto entity : view)
			{
				UUID entityID = m_Registry.get<IdComponent>(entity).ID;
				Entity e = { entity, this };
				if (ScriptEngine::ModuleExists(e.GetComponent<BehaviourComponent>().ModuleName))
					ScriptEngine::OnUpdateEntity(m_SceneID, entityID, ts);
			}
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


		

		SceneCamera& mainCamera = m_CameraStack->MainCamera();
		if (mainCamera.IsActive())
		{
			Renderer2D::BeginScene(mainCamera.ViewProjectionMatrix());

			auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
			for (auto entity : group)
			{
				auto [trans, sprRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				
				Ref<SubTexture2D> subTex = AssetManager::GetAsset<SubTexture2D>(sprRenderer.Sprite);
				if (subTex.get())
				{
					Renderer2D::DrawQuad(trans.GetTransform(), subTex);
				}
				else
				{
					glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
					Renderer2D::DrawQuad(trans.Translation, trans.Scale, color);
				}
			}

			Renderer2D::EndScene();
		}
	}





	void Scene::OnUpdateEditor(Timestep ts, glm::mat4 cameraViewProj)
	{
		Renderer2D::BeginScene(cameraViewProj);

		auto group = m_Registry.group<SpriteRendererComponent>(entt::get<TransformComponent>);
		for (auto entity : group)
		{
			auto [trans, sprRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);

			Ref<SubTexture2D> subTex = AssetManager::GetAsset<SubTexture2D>(sprRenderer.Sprite);
			if (subTex.get())
			{
				Renderer2D::DrawQuad(trans.GetTransform(), subTex);
			}
			else 
			{
				glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
				Renderer2D::DrawQuad(trans.Translation, trans.Scale, color);
			}
		}

		Renderer2D::EndScene();
	}



	Entity Scene::CreateEntity(const std::string& name)
	{
		UUID id = {};
		return CreateEntity(name, id);
	}

	Entity Scene::CreateEntity(const std::string& name, UUID id)
	{
		const std::string& goName = name.empty() ? DefaultEntityName : name;

		auto entity = Entity{ m_Registry.create(), this };
		auto& idComp = entity.AddComponent<IdComponent>();
		idComp.ID = id;

		XA_CORE_ASSERT(m_EntityMap.find(idComp.ID) == m_EntityMap.end(), "Repeated entity ID!");

		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(goName);

		XA_CORE_TRACE("Registering entity to the EntityMap. Id = {}", idComp.ID);
		m_EntityMap[idComp.ID] = entity;
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
