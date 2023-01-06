#include "SceneHierarchyPanel.h"

#include "../EditorLayer.h"
#include "../EditorNames.h"

#include "Xaloc/Editor/PropertyDrawer.h"

#include "Xaloc/Renderer/Texture.h"
#include "Xaloc/Renderer/SubTexture2D.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>

// TODO:
// - Eventually change imgui node IDs to be entity/asset GUID

namespace Xaloc {
	
	SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* editorLayer, const Ref<Scene>& scene)
		: m_EditorLayer(editorLayer), m_Scene(scene)
	{
	}

	void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene)
	{
		m_Scene = scene;
		m_SelectionContext = {};
		m_ShowProperties = false;
	}

	void SceneHierarchyPanel::SetSelected(Entity entity)
	{
		m_SelectionContext = entity;
		m_ShowProperties = true;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		// ======================================== SCENE HIERARCHY ======================================== //
		
		ImGui::Begin(EditorNames::Windows::SCENE_HIERARCHY);
		uint32_t entityCount = 0, meshCount = 0;
		m_Scene->m_Registry.each([&](auto entity)
			{
				DrawEntityNode(Entity(entity, m_Scene.get()));
			});

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
	

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		const char* name = "Unnamed Entity";
		if (entity.HasComponent<TagComponent>())
			name = entity.GetComponent<TagComponent>().Tag.c_str();

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
	







	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

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
				uiFunction(component);
				ImGui::TreePop();
			}
			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}


	

	

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		ImGui::AlignTextToFramePadding();
		
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, 256);
			memcpy(buffer, tag.c_str(), tag.length());
			if (ImGui::InputText("##Tag", buffer, 256))
			{
				tag = std::string(buffer);
			}
		}
		else ImGui::TextDisabled("Unnamed entity");

		if (entity.HasComponent<IdComponent>())
		{
			UUID id = entity.GetComponent<IdComponent>().ID;
			ImGui::SameLine();
			ImGui::TextDisabled("%llx", id);
		}
		


		DrawComponent<TransformComponent>("Transform", entity, [&entity](auto& tc)
		{
			PropertyDrawer::BeginPropertyGrid();

			PropertyDrawer::Vec3("Translation", tc.Translation);
			glm::vec3 rotation = glm::degrees(tc.Rotation);
			PropertyDrawer::Vec3("Rotation", rotation);
			tc.Rotation = glm::radians(rotation);

			/*
			if (entity.HasComponent<CameraComponent>())
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
				PropertyDrawer::Vec3("Scale", tc.Scale, 1.0f);
				ImGui::PopItemFlag();
				ImGui::PopStyleVar();
			}
			else */
			PropertyDrawer::Vec3("Scale", tc.Scale, 1.0f);

			PropertyDrawer::EndPropertyGrid();
		});

		/*
		if (entity.HasComponent<TransformComponent>())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{				
				bool change = false;
				auto [translation, rotationQuat, scale] = GetTransformDecomposition(tc);
				glm::vec3 rotation = glm::degrees(glm::eulerAngles(rotationQuat));

				PropertyDrawer::BeginPropertyGrid();
				
				change |=  PropertyDrawer::Vec3("Translation", translation);

				if (entity.HasComponent<CameraComponent>())
				{
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
					change |= PropertyDrawer::Vec3("Scale", scale);
					ImGui::PopItemFlag();
					ImGui::PopStyleVar();
				}
				else
				{
					change |= PropertyDrawer::Vec3("Scale", scale);
				}
				
				change |= PropertyDrawer::Vec3("Rotation", rotation);


				if (change)
				{
					tc.Transform = glm::translate(glm::mat4(1.0f), translation) *
						glm::toMat4(glm::quat(glm::radians(rotation))) *
						glm::scale(glm::mat4(1.0f), scale);
				}

				PropertyDrawer::EndPropertyGrid();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		*/

		
		DrawComponent<SceneComponent>("Scene", entity, [&](auto& sc)
		{
			PropertyDrawer::BeginPropertyGrid();

			ImGui::Text("Scene ID");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::Text("%llx", sc.SceneID);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			if (PropertyDrawer::String("Name", sc.Name))
			{
				m_Scene->m_Name = sc.Name;
			}

			PropertyDrawer::EndPropertyGrid();
		});


		DrawComponent<ColliderComponent>("Collider", entity, [](auto& cc)
		{
				PropertyDrawer::BeginPropertyGrid();
				PropertyDrawer::Vec2("Size", cc.Size, 0.1f, 0.0f);
				PropertyDrawer::EndPropertyGrid();
		});
		

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [&](auto& src)
		{
			auto iconRef = m_EditorLayer->GetIcons()->GetAssetIcon(AssetType::Sprite)->GetRendererID();

			PropertyDrawer::BeginPropertyGrid();

			AssetHandle texAsset = src.Sprite;
			if (PropertyDrawer::Asset("Sprite", AssetType::Sprite, texAsset, (void*)iconRef))
			{
				src.Sprite = texAsset;
			}

			PropertyDrawer::EndPropertyGrid();
		});


		DrawComponent<BehaviourComponent>("Behaviour", entity, [&entity](auto& sc)
		{
			UUID id = entity.GetComponent<IdComponent>().ID;
			
			PropertyDrawer::BeginPropertyGrid();
			std::string oldName = sc.ModuleName;
			if (PropertyDrawer::String("Module Name", sc.ModuleName, !ScriptEngine::ModuleExists(sc.ModuleName))) // TODO: no live edit
			{
				// Shutdown old script
				if (ScriptEngine::ModuleExists(oldName))
					ScriptEngine::ShutdownBehaviourEntity(entity, oldName);

				if (ScriptEngine::ModuleExists(sc.ModuleName))
					ScriptEngine::InitBehaviourEntity(entity);
			}

			// Public Fields
			if (ScriptEngine::ModuleExists(sc.ModuleName))
			{
				EntityInstanceFields& entityInstanceFields = ScriptEngine::GetEntityInstanceFields(entity.GetScene()->GetID(), id);
				auto& moduleFieldMap = entityInstanceFields.ModuleFieldMap;
				if (moduleFieldMap.find(sc.ModuleName) != moduleFieldMap.end())
				{
					auto& publicFields = moduleFieldMap.at(sc.ModuleName);
					for (auto& [name, field] : publicFields)
					{
						switch (field.Type)
						{
						case FieldType::Int:
						{
							int value = field.GetValue<int>();
							if (PropertyDrawer::Int(field.Name.c_str(), value))
								field.SetValue(value);
							break;
						}
						case FieldType::Float:
						{
							float value = field.GetValue<float>();
							if (PropertyDrawer::Float(field.Name.c_str(), value, 0.2f))
								field.SetValue(value);
							break;
						}
						case FieldType::Vec2:
						{
							glm::vec2 value = field.GetValue<glm::vec2>();
							if (PropertyDrawer::Vec2(field.Name.c_str(), value, 0.2f))
								field.SetValue(value);
							break;
						}
						case FieldType::Vec3:
						{
							glm::vec3 value = field.GetValue<glm::vec3>();
							if (PropertyDrawer::Vec3(field.Name.c_str(), value, 0.2f))
								field.SetValue(value);
							break;
						}
						case FieldType::Vec4:
						{
							glm::vec4 value = field.GetValue<glm::vec4>();
							if (PropertyDrawer::Vec4(field.Name.c_str(), value, 0.2f))
								field.SetValue(value);
							break;
						}
						}
					}
				}
			}

			PropertyDrawer::EndPropertyGrid();
		});





		ImGui::Separator();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Behaviour"))
			{
				if (!m_SelectionContext.HasComponent<BehaviourComponent>())
					m_SelectionContext.AddComponent<BehaviourComponent>();
				else XA_CORE_WARN("This entity already has the Behaviour Component!");
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Collider"))
			{
				if (!m_SelectionContext.HasComponent<ColliderComponent>())
					m_SelectionContext.AddComponent<ColliderComponent>();
				else XA_CORE_WARN("This entity already has the Collider Component!");
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
				{
					m_SelectionContext.AddComponent<SpriteRendererComponent>();
				}
				else XA_CORE_WARN("This entity already has the Sprite Renderer Component!");
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		
	}


}