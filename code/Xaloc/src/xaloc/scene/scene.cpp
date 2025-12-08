#include "xapch.h"
#include "xaloc/scene/scene.h"
#include "xaloc/scene/components.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/serializer.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/renderer2D.h"
#include "xaloc/renderer/camera.h"
#include "xaloc/renderer/cameras/editor.h"
#include "xaloc/scripting/engine.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>


namespace Xaloc
{

static const std::string DefaultEntityName = "New Entity";

std::unordered_map<ZGuid, ZScene*> s_ActiveScenes;

void OnTransformConstruct(entt::registry& registry, entt::entity entity)
{
	XA_CORE_TRACE("New Transform component registered!");
}

void OnTagConstruct(entt::registry& registry, entt::entity entity)
{
	auto& tag = registry.get<CmpTag>(entity);
	XA_CORE_TRACE("New Tag component registered: {0}", tag.sTag);
}

void OnBehaviourComponentConstruct(entt::registry& registry, entt::entity e)
{
	XA_CORE_TRACE("");

	auto view = registry.view<CmpScene>();
	ZGuid sceneID = registry.get<CmpScene>(view.front()).idScene;
	ZScene* scene = s_ActiveScenes[sceneID];

	auto entityID = registry.get<CmpId>(e).idValue;
	XA_CORE_ASSERT(scene->m_EntityMap.find(entityID) != scene->m_EntityMap.end(), "Could not find Behaviour entity!");

	Scripting::ZScriptEngine::InitBehaviourEntity(scene->m_EntityMap.at(entityID));
}

void ZScene::Save(const ZRef<ZScene>& scene, const std::string& filename)
{
	pugi::xml_document doc = ZSceneSerializer::Serialize(scene);
	bool success = doc.save_file(filename.c_str());

	if (!success)
	{
		XA_CORE_ERROR("Could not save scene in path '{}'", filename);
	}
	else XA_CORE_INFO("Scene saved in path '{}'", filename);
}

ZRef<ZScene> ZScene::Load(const std::string& filename)
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

	return ZSceneSerializer::Deserialize(doc);
}

ZScene::ZScene(const std::string& name)
	: m_Name(name)
{
	Init();
}

ZScene::ZScene(const std::string& name, ZGuid id)
	: m_Name(name), m_SceneID(id)
{
	Init();
}

ZScene::~ZScene()
{
	m_Registry.clear();
	s_ActiveScenes.erase(m_SceneID);
}

void ZScene::Init()
{
	m_pCameraStack = CreateRef<ZCameraStack>();

	m_Registry.on_construct<CmpTransform>().connect<&OnTransformConstruct>();
	m_Registry.on_construct<CmpTag>().connect<&OnTagConstruct>();
	m_Registry.on_construct<CmpBehaviour>().connect<&OnBehaviourComponentConstruct>();

	m_SceneEntity = m_Registry.create();
	m_Registry.emplace<CmpScene>(m_SceneEntity, m_SceneID, m_Name);
	m_Registry.emplace<CmpTag>(m_SceneEntity, "Scene Entity");

	s_ActiveScenes[m_SceneID] = this;

	auto test = m_Registry.create();
	m_Registry.emplace<CmpTag>(test, "Test Collider Entity");
	m_Registry.emplace<CmpTransform>(test);
	m_Registry.emplace<CmpCollider>(test, glm::vec2(1.0f));
}

void ZScene::StartRuntime()
{
	// TODO we should know if we are on runtime or not, since ScriptEngine::InstantiateEntityClass will need to be called on behaviors created AFTER this point

	XA_CORE_INFO("Starting Script Runtime...");

	auto view = m_Registry.view<CmpBehaviour>();
	for (auto entity : view)
	{
		ZEntity e = { entity, this };
		if (Scripting::ZScriptEngine::ModuleExists(e.GetComponent<CmpBehaviour>().sModuleName))
		{
			Scripting::ZScriptEngine::InstantiateEntityClass(e);
		}
		else
		{
			XA_CORE_WARN(
				"Could not initialize behaviour '{0}'. Module does not exist.",
				e.GetComponent<CmpBehaviour>().sModuleName);
		}
	}
}

void ZScene::OnUpdateRuntime(ZTimestep ts)
{
	// Update all entities
	{
		auto view = m_Registry.view<CmpBehaviour>();
		for (auto entity : view)
		{
			ZGuid entityID = m_Registry.get<CmpId>(entity).idValue;
			ZEntity e = { entity, this };
			if (Scripting::ZScriptEngine::ModuleExists(e.GetComponent<CmpBehaviour>().sModuleName))
			{
				Scripting::ZScriptEngine::OnUpdateEntity(m_SceneID, entityID, ts);
			}
		}
	}

	// For more info on entt groups:
	// https://skypjack.github.io/2019-04-12-entt-tips-and-tricks-part-1/
	// https://github.com/skypjack/entt/wiki/Crash-Course:-entity-component-system#groups

	// Physics
	{
		// Transform component will be owned by the Rendering group
		auto group = m_Registry.group<CmpCollider>(entt::get<CmpTransform>);

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
			//auto [transformComponent, colliderComponent] = view.get<CmpTransform, ColliderComponent>(entity);
			//auto trans = transformComponent;
			//auto col = colliderComponent;

			// TODO post-process
		}
	}

	SSceneCamera& rMainCamera = m_pCameraStack->MainCamera();
	if (rMainCamera.IsActive())
	{
		ZRenderer2D::BeginScene(rMainCamera.ViewProjectionMatrix());

		auto group = m_Registry.group<CmpSpriteRenderer>(entt::get<CmpTransform>);
		for (auto entity : group)
		{
			auto [trans, sprRenderer] = group.get<CmpTransform, CmpSpriteRenderer>(entity);

			ZRef<ZSubTexture2D> subTex = ZAssetManager::GetAsset<ZSubTexture2D>(sprRenderer.Sprite);
			if (subTex.get())
			{
				ZRenderer2D::DrawQuad(trans.GetTransform(), subTex);
			}
			else
			{
				glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
				ZRenderer2D::DrawQuad(trans.Translation, trans.Scale, color);
			}
		}

		ZRenderer2D::EndScene();
	}
}

void ZScene::OnUpdateEditor(ZTimestep ts, glm::mat4 cameraViewProj)
{
	ZRenderer2D::BeginScene(cameraViewProj);

	auto group = m_Registry.group<CmpSpriteRenderer>(entt::get<CmpTransform>);
	for (auto entity : group)
	{
		auto [trans, sprRenderer] = group.get<CmpTransform, CmpSpriteRenderer>(entity);

		ZRef<ZSubTexture2D> subTex = ZAssetManager::GetAsset<ZSubTexture2D>(sprRenderer.Sprite);
		if (subTex.get())
		{
			ZRenderer2D::DrawQuad(trans.GetTransform(), subTex);
		}
		else 
		{
			glm::vec4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
			ZRenderer2D::DrawQuad(trans.Translation, trans.Scale, color);
		}
	}

	ZRenderer2D::EndScene();
}

ZEntity ZScene::CreateEntity(const std::string& name)
{
	ZGuid id = {};
	return CreateEntity(name, id);
}

ZEntity ZScene::CreateEntity(const std::string& name, ZGuid id)
{
	const std::string& goName = name.empty() ? DefaultEntityName : name;

	ZEntity entity = ZEntity{ m_Registry.create(), this };
	auto& idComp = entity.AddComponent<CmpId>();
	idComp.idValue = id;

	XA_CORE_ASSERT(m_EntityMap.find(idComp.idValue) == m_EntityMap.end(), "Repeated entity ID!");

	entity.AddComponent<CmpTransform>();
	entity.AddComponent<CmpTag>(goName);

	XA_CORE_TRACE("Registering entity to the EntityMap. Id = {}", idComp.idValue.Value());
	m_EntityMap[idComp.idValue] = entity;
	return entity;
}
	
void ZScene::DestroyEntity(ZEntity entity)
{
	m_Registry.destroy(entity.m_EntityHandle);
}

void ZScene::OnEvent(ZEvent& rEvent)
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