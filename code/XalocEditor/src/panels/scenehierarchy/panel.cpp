#include "editor/module.h"
#include "editor/names.h"
#include "panels/scenehierarchy/panel.h"
#include "tooling/propertydrawer.h"
#include "xaloc/renderer/resource/texture.h"
#include "xaloc/renderer/resource/subtexture.h"
#include "xaloc/scripting/engine.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>

using namespace Xaloc;

// TODO:
// - Eventually change imgui node IDs to be entity/asset GUID

namespace Editor
{

ZPanelSceneHierarchy::ZPanelSceneHierarchy(ZModuleEditor* pEditor, const ZRef<ZScene>& scene)
	: m_pEditor(pEditor), m_Scene(scene)
{
}

void ZPanelSceneHierarchy::SetScene(const ZRef<ZScene>& scene)
{
	m_Scene = scene;
	m_SelectionContext = {};
	m_ShowProperties = false;
}

void ZPanelSceneHierarchy::SetSelected(ZEntity entity)
{
	m_SelectionContext = entity;
	m_ShowProperties = true;
}

void ZPanelSceneHierarchy::OnImGuiRender()
{
	// ======================================== SCENE HIERARCHY ======================================== //
	ImGui::Begin(Names::Windows::SCENE_HIERARCHY);
	uint32_t entityCount = 0, meshCount = 0;
	auto view = m_Scene->WriteRegistry().view<entt::entity>();
	for (auto entity : view)
	{
		DrawEntityNode(ZEntity(entity, m_Scene.get()));
	};

	if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			m_Scene->CreateEntity("Empty Entity");
		}
		ImGui::EndPopup();
	}
	ImGui::End();

	// =========================================== PROPERTIES ========================================== //
	ImGui::Begin("Properties");
	if (m_ShowProperties)
	{
		DrawComponents(m_SelectionContext);
		
		// TODO add component
	}
	ImGui::End();
}

void ZPanelSceneHierarchy::DrawEntityNode(ZEntity entity)
{
	const char* name = "Unnamed Entity";
	if (entity.HasComponent<CmpTag>())
		name = entity.GetComponent<CmpTag>().sTag.c_str();

	ImGuiTreeNodeFlags node_flags = (entity == m_SelectionContext ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, node_flags, name);
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
		m_ShowProperties = true;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete"))
			entityDeleted = true;

		ImGui::EndPopup();
	}
	if (opened)
	{
		ImGui::TreePop();
	}

	// Defer deletion until end of node UI
	if (entityDeleted)
	{
		m_Scene->DestroyEntity(entity);
		if (entity == m_SelectionContext)
		{
			m_SelectionContext = {};
			m_ShowProperties = false;
		}
	}
}

template<typename T>
static void DrawComponentGeneric(ZPanelSceneHierarchy* pPanel, const std::string& name, ZEntity entity)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowOverlap | ImGuiTreeNodeFlags_FramePadding;

	if (entity.HasComponent<T>())
	{
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->LegacySize + GImGui->Style.FramePadding.y * 2.0f;

		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

		ImGui::PopStyleVar();
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) // TODO "\u2261" instead of "+"
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			pPanel->DrawComponent(component, entity);
			ImGui::TreePop();
		}
		if (removeComponent)
		{
			entity.RemoveComponent<T>();
		}
	}
}

void ZPanelSceneHierarchy::DrawComponent(CmpTransform& rTransform, ZEntity entity)
{
	ZPropertyDrawer imgui("scenehierarchy_entity_transform");
	imgui.Begin();

	imgui.Vec3("Translation", rTransform.Translation);
	glm::vec3 rotation = glm::degrees(rTransform.Rotation);
	imgui.Vec3("Rotation", rotation);
	rTransform.Rotation = glm::radians(rotation);

	/*
	if (entity.HasComponent<CameraComponent>())
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		imgui.Vec3("Scale", tc.Scale, 1.0f);
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}
	else */
	imgui.Vec3("Scale", rTransform.Scale, 1.0f);

	imgui.End();
}

void ZPanelSceneHierarchy::DrawComponent(CmpScene& rScene, ZEntity entity)
{
	ZPropertyDrawer imgui("scenehierarchy_entity_scene");
	imgui.Begin();

	ImGui::Text("Scene ID");
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);
	ImGui::Text("%llx", rScene.idScene);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	if (imgui.String("Name", rScene.sName))
	{
		m_Scene->SetName(rScene.sName.c_str());
	}

	imgui.End();
}

void ZPanelSceneHierarchy::DrawComponent(CmpCollider& rCollider, ZEntity entity)
{
	ZPropertyDrawer imgui("scenehierarchy_entity_collider");
	imgui.Begin();
	imgui.Vec2("Size", rCollider.Size, 0.1f, 0.0f);
	imgui.End();
}

void ZPanelSceneHierarchy::DrawComponent(CmpSpriteRenderer& rSpriteRenderer, ZEntity entity)
{
	auto iconRef = m_pEditor->GetIcons()->GetAssetIcon(EAssetType::Sprite)->GetRendererID();

	ZPropertyDrawer imgui("scenehierarchy_entity_spriterenderer");
	imgui.Begin();

	ZAssetHandle texAsset = rSpriteRenderer.Sprite;
	if (imgui.Asset("Sprite", EAssetType::Sprite, texAsset, (void*)iconRef))
	{
		rSpriteRenderer.Sprite = texAsset;
	}

	imgui.End();
}

void ZPanelSceneHierarchy::DrawComponent(CmpBehaviour& rBehaviour, ZEntity entity)
{
	ZGuid id = entity.GetComponent<CmpId>().idValue;

	ZPropertyDrawer imgui("scenehierarchy_entity_behaviour");
	imgui.Begin();
	const bool bModuleExists = Scripting::ZScriptEngine::ModuleExists(rBehaviour.sModuleName);
	const std::string& sOldName = rBehaviour.sModuleName;

	// TODO: do this better
	//   - make this its own CmpBehaviourEditor class
	//   - add a "set" button that unbinds old module name and binds the new one
	//   - cache mono types and fields

	if (imgui.String(
		"Module Name",
		rBehaviour.sModuleName,
		!bModuleExists)) // TODO: no live edit
	{
		// Shutdown old script
		if (bModuleExists)
		{
			Scripting::ZScriptEngine::ShutdownBehaviourEntity(entity, sOldName);
		}
		if (Scripting::ZScriptEngine::ModuleExists(rBehaviour.sModuleName))
		{
			Scripting::ZScriptEngine::InitBehaviourEntity(entity);
		}
	}

	// Public Fields
	if (bModuleExists)
	{
		const Scripting::SEntityInstanceFields& entityInstanceFields =
			Scripting::ZScriptEngine::GetEntityInstanceFields(entity.GetScene()->GetID(), id);
		const Scripting::ZScriptModuleFieldMap& moduleFieldMap = entityInstanceFields.ModuleFieldMap;

		if (moduleFieldMap.find(rBehaviour.sModuleName) != moduleFieldMap.end())
		{
			const auto& publicFields = moduleFieldMap.at(rBehaviour.sModuleName);
			for (const auto& [sName, field] : publicFields)
			{
				switch (field.Type)
				{
				case Scripting::EFieldType::Int:
				{
					int value = field.GetValue<int>();
					if (imgui.Int(field.Name.c_str(), value))
					{
						field.SetValue(value);
					}
					break;
				}
				case Scripting::EFieldType::Float:
				{
					float value = field.GetValue<float>();
					if (imgui.Float(field.Name.c_str(), value, 0.2f))
						field.SetValue(value);
					break;
				}
				case Scripting::EFieldType::Vec2:
				{
					glm::vec2 value = field.GetValue<glm::vec2>();
					if (imgui.Vec2(field.Name.c_str(), value, 0.2f))
						field.SetValue(value);
					break;
				}
				case Scripting::EFieldType::Vec3:
				{
					glm::vec3 value = field.GetValue<glm::vec3>();
					if (imgui.Vec3(field.Name.c_str(), value, 0.2f))
						field.SetValue(value);
					break;
				}
				case Scripting::EFieldType::Vec4:
				{
					glm::vec4 value = field.GetValue<glm::vec4>();
					if (imgui.Vec4(field.Name.c_str(), value, 0.2f))
						field.SetValue(value);
					break;
				}
				}
			}
		}
	}

	imgui.End();
}

void ZPanelSceneHierarchy::DrawComponents(ZEntity entity)
{
	ImGui::AlignTextToFramePadding();
		
	if (entity.HasComponent<CmpTag>())
	{
		auto& tag = entity.GetComponent<CmpTag>().sTag;
		char buffer[256];
		memset(buffer, 0, 256);
		memcpy(buffer, tag.c_str(), tag.length());
		if (ImGui::InputText("##Tag", buffer, 256))
		{
			tag = std::string(buffer);
		}
	}
	else
	{
		ImGui::TextDisabled("Unnamed entity");
	}

	if (entity.HasComponent<CmpId>())
	{
		ZGuid id = entity.GetComponent<CmpId>().idValue;
		ImGui::SameLine();
		ImGui::TextDisabled("%llx", id);
	}

	DrawComponentGeneric<CmpTransform>(this, "transform", entity);
	DrawComponentGeneric<CmpScene>(this, "scene", entity);
	DrawComponentGeneric<CmpCollider>(this, "collider", entity);
	DrawComponentGeneric<CmpSpriteRenderer>(this, "sprite renderer", entity);
	DrawComponentGeneric<CmpBehaviour>(this, "behaviour", entity);

	ImGui::Separator();
	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}
	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Behaviour"))
		{
			if (!m_SelectionContext.HasComponent<CmpBehaviour>())
				m_SelectionContext.AddComponent<CmpBehaviour>();
			else XA_CORE_WARN("This entity already has the Behaviour Component!");
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Collider"))
		{
			if (!m_SelectionContext.HasComponent<CmpCollider>())
			{
				m_SelectionContext.AddComponent<CmpCollider>();
			}
			else
			{
				XA_CORE_WARN("This entity already has the Collider Component!");
			}
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::MenuItem("Sprite Renderer"))
		{
			if (!m_SelectionContext.HasComponent<CmpSpriteRenderer>())
			{
				m_SelectionContext.AddComponent<CmpSpriteRenderer>();
			}
			else XA_CORE_WARN("This entity already has the Sprite Renderer Component!");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();
}

}