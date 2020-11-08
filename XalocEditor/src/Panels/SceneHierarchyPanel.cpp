#include "SceneHierarchyPanel.h"

#include "PropertyDrawer.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <cstring>

// TODO:
// - Eventually change imgui node IDs to be entity/asset GUID

namespace Xaloc {
	
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
		: m_Scene(scene)
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
		
		ImGui::Begin("Scene Hierarchy");
		uint32_t entityCount = 0, meshCount = 0;
		m_Scene->m_Registry.each([&](auto entity)
			{
				DrawEntityNode(Entity(entity, m_Scene.get()));
			});

		if (ImGui::BeginPopupContextWindow(0, 1, false))
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
		else
		{
			ImGui::TextDisabled("Unnamed entity");
		}
		
		if (entity.HasComponent<IdComponent>())
		{
			UUID id = entity.GetComponent<IdComponent>().ID;
			ImGui::SameLine();
			ImGui::TextDisabled("%llx", id);
		}
		ImGui::Separator();



		
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


		if (entity.HasComponent<SceneComponent>())
		{
			auto& sc = entity.GetComponent<SceneComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity), ImGuiTreeNodeFlags_DefaultOpen, "Scene"))
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
				ImGui::TreePop();
			}
			ImGui::Separator();
		}




		
		if (entity.HasComponent<CameraComponent>())
		{			
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(CameraComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& cc = entity.GetComponent<CameraComponent>();
				auto& camera = cc.Camera;
				
				PropertyDrawer::BeginPropertyGrid();
				PropertyDrawer::Int("Priority", cc.Priority);

				
				const std::vector<std::string> projectionTypeStrings = { "Perspective", "Orthographic" };
				const char* currentProjectionType = projectionTypeStrings[(int)camera.GetProjectionType()].c_str();
				int projIndex;

				if (PropertyDrawer::ComboBox("Projection", projectionTypeStrings, currentProjectionType, &projIndex))
				{
					currentProjectionType = projectionTypeStrings[(int)camera.GetProjectionType()].c_str();
					camera.SetProjectionType((SceneCamera::ProjectionType)projIndex);
				}
				
				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (PropertyDrawer::Float("Priority", perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (PropertyDrawer::Float("Near", perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (PropertyDrawer::Float("Far", perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (PropertyDrawer::Float("Size", orthoSize, 0.1f, 0.1f))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();
					
					if (PropertyDrawer::Float("Near", orthoNear, 0.1f, -10000.0f, orthoFar - 0.1f))
						camera.SetOrthographicNearClip(orthoNear);
					if (PropertyDrawer::Float("Far", orthoFar, 0.1f, orthoNear + 0.1f, 10000.0f))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &cc.FixedAspectRatio);
				}
				
				PropertyDrawer::EndPropertyGrid();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}




		
		if (entity.HasComponent<ColliderComponent>())
		{
			auto& cc = entity.GetComponent<ColliderComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(ColliderComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Collider"))
			{
				PropertyDrawer::BeginPropertyGrid();
				PropertyDrawer::Vec2("Size", cc.Size, 0.1f, 0.0f);
				PropertyDrawer::EndPropertyGrid();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(SpriteRendererComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				PropertyDrawer::BeginPropertyGrid();
				PropertyDrawer::String("Asset ID", src.SubTexture->GetTexture()->AssetID().c_str());

				PropertyDrawer::EndPropertyGrid();
				if (ImGui::Button("Apply"))
				{
					// TODO
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<BehaviourComponent>() && entity.HasComponent<IdComponent>())
		{
			UUID id = entity.GetComponent<IdComponent>().ID;
			
			auto& sc = entity.GetComponent<BehaviourComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(BehaviourComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Script"))
			{
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
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

	}

}