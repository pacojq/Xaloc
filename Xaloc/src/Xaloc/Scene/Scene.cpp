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


	

	
	void print_doc(const char* message, const pugi::xml_document& doc, const pugi::xml_parse_result& result)
	{
		std::cout
			<< message
			<< "\t: load result '" << result.description() << "'"
			<< ", first character of root name: U+" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << pugi::as_wide(doc.first_child().name())[0]
			<< ", year: " << doc.first_child().first_child().child_value()
			<< std::endl;
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
		
		return SceneSerializer::Deserialize(doc, result);
	}


	
	
	Scene::Scene(const std::string& name)
		: m_Name(name), m_SceneID(s_SceneIDCounter++)
	{
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();
		m_Registry.on_construct<TagComponent>().connect<&OnTagConstruct>();
		m_Registry.on_construct<BehaviourComponent>().connect<&OnBehaviourComponentConstruct>();

		m_SceneEntity = m_Registry.create();
		m_Registry.emplace<SceneComponent>(m_SceneEntity, m_SceneID);
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