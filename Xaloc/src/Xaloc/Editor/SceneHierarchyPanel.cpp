#include "xapch.h"
#include "SceneHierarchyPanel.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include <imgui.h>
#include <limits>

// TODO:
// - Eventually change imgui node IDs to be entity/asset GUID

namespace Xaloc {

	#define FLOAT_MIN std::numeric_limits<float>::min()
	#define FLOAT_MAX std::numeric_limits<float>::max()
	

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
	

	static int s_UIContextID = 0;
	static uint32_t s_Counter = 0;
	static char s_IDBuffer[16];

	static void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	static void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	static void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}

	static bool Property(const char* label, std::string& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy(buffer, value.c_str());

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::InputText(s_IDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, const char* value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::InputText(s_IDBuffer, (char*)value, 256, ImGuiInputTextFlags_ReadOnly))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, int& value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, float& value, float delta = 0.1f, float min = FLOAT_MIN, float max = FLOAT_MAX)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float min = FLOAT_MIN, float max = FLOAT_MAX)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(value), delta, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	static void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
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

			ImGui::Separator();
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto [translation, rotation, scale] = GetTransformDecomposition(tc);

				ImGui::Columns(2);
				ImGui::Text("Translation");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				if (ImGui::DragFloat3("##translation", glm::value_ptr(translation), 0.25f))
				{
					tc.Transform[3] = glm::vec4(translation, 1.0f);
				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Text("Scale");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);

				if (ImGui::DragFloat3("##scale", glm::value_ptr(scale), 0.25f))
				{

				}

				ImGui::PopItemWidth();
				ImGui::NextColumn();

				ImGui::Columns(1);

				// ImGui::Text("Translation: %.2f, %.2f, %.2f", translation.x, translation.y, translation.z);
				// ImGui::Text("Scale: %.2f, %.2f, %.2f", scale.x, scale.y, scale.z);
				ImGui::TreePop();
			}
			ImGui::Separator();
		}


		if (entity.HasComponent<SceneComponent>())
		{
			auto& sc = entity.GetComponent<SceneComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity), ImGuiTreeNodeFlags_DefaultOpen, "Scene"))
			{
				BeginPropertyGrid();

				ImGui::Text("ID");
				ImGui::NextColumn();
				ImGui::PushItemWidth(-1);
				ImGui::Text(std::to_string(sc.SceneID).c_str());
				ImGui::PopItemWidth();
				ImGui::NextColumn();

				if (Property("Name", sc.Name))
				{
					m_Scene->m_Name = sc.Name;
				}

				EndPropertyGrid();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		/* TODO camera

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cc = entity.GetComponent<CameraComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(CameraComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{

				ImGui::TreePop();
			}
			ImGui::Separator();
		}
		*/

		if (entity.HasComponent<ColliderComponent>())
		{
			auto& cc = entity.GetComponent<ColliderComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(ColliderComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Collider"))
			{
				BeginPropertyGrid();
				Property("Size", cc.Size, 0.1f, 0.0f);
				EndPropertyGrid();
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& src = entity.GetComponent<SpriteRendererComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(SpriteRendererComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				BeginPropertyGrid();
				Property("Asset ID", src.SubTexture->GetTexture()->AssetID().c_str());

				EndPropertyGrid();
				if (ImGui::Button("Apply"))
				{
					// TODO
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

		if (entity.HasComponent<BehaviourComponent>())
		{
			auto& sc = entity.GetComponent<BehaviourComponent>();
			if (ImGui::TreeNodeEx((void*)((uint32_t)entity | typeid(BehaviourComponent).hash_code()), ImGuiTreeNodeFlags_DefaultOpen, "Script"))
			{
				BeginPropertyGrid();
				Property("Module Name", sc.ModuleName.c_str());

				// Public Fields
				auto& fieldMap = ScriptEngine::GetFieldMap();
				if (fieldMap.find(sc.ModuleName) != fieldMap.end())
				{
					auto& publicFields = fieldMap.at(sc.ModuleName);
					for (auto& field : publicFields)
					{
						switch (field.Type)
						{
						case FieldType::Int:
						{
							int value = field.GetValue<int>();
							if (Property(field.Name.c_str(), value))
							{
								field.SetValue(value);
							}
							break;
						}
						case FieldType::Float:
						{
							float value = field.GetValue<float>();
							if (Property(field.Name.c_str(), value, 0.2f))
							{
								field.SetValue(value);
							}
							break;
						}
						case FieldType::Vec2:
						{
							glm::vec2 value = field.GetValue<glm::vec2>();
							if (Property(field.Name.c_str(), value, 0.2f))
							{
								field.SetValue(value);
							}
							break;
						}
						}
					}
				}

				EndPropertyGrid();
				if (ImGui::Button("Run Script"))
				{
					ScriptEngine::OnCreateEntity(entity);
				}
				ImGui::TreePop();
			}
			ImGui::Separator();
		}

	}

}