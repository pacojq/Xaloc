#include "xapch.h"
#include "EditorMenuBar.h"

#include "EditorLayer.h"

#include "Xaloc/Files/FileSystem.h"

#include "Xaloc/Core/Input/Input.h"

#include "imgui/imgui_internal.h"

namespace Xaloc {


	EditorMenuBar::EditorMenuBar(EditorLayer* editorLayer)
		: m_EditorLayer(editorLayer)
	{
	}



	void EditorMenuBar::OnImGuiRender()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Xaloc"))
			{
				if (ImGui::MenuItem("Exit")) Xaloc::Application::Get().Close();
				ImGui::EndMenu();
			}

			DoMenuScene();
			DoMenuView();
			DoMenuHelp();

			ImGui::EndMenuBar();
		}
	}



	void EditorMenuBar::DoMenuScene()
	{
		if (ImGui::BeginMenu("Scene"))
		{
			//if (ImGui::MenuItem("Save"))
			//{
				// TODO
			//}
			if (ImGui::MenuItem("Save as..."))
			{
				std::string filename = FileSystem::SaveFileDialog(s_FilenameFilter);
				if (filename != "")
					Scene::Save(m_EditorLayer->m_Scene, filename);
			}
			if (ImGui::MenuItem("Load..."))
			{
				std::string filename = FileSystem::OpenFileDialog(s_FilenameFilter);
				if (filename != "")
				{
					m_EditorLayer->m_Scene.reset();
					m_EditorLayer->m_Scene = Scene::Load(filename);
					m_EditorLayer->m_SceneHierarchyPanel->SetScene(m_EditorLayer->m_Scene);
				}
			}
			ImGui::EndMenu();
		}
	}


	void EditorMenuBar::DoMenuView()
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console")) Application::Get().GetImGuiLayer()->ShowConsole();
			if (ImGui::MenuItem("Profiler")) Application::Get().GetImGuiLayer()->ShowProfiler();
			if (ImGui::MenuItem("Render Stats")) m_EditorLayer->m_ShowWindowRenderStats = true;

			ImGui::EndMenu();
		}
	}

	void EditorMenuBar::DoMenuHelp()
	{
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				//m_EditorLayer->m_ShowAboutWindow = true;
			}
			ImGui::EndMenu();
		}
	}

}
