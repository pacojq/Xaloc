#include "xapch.h"
#include "editor/menubar.h"
#include "editor/module.h"
#include "xaloc/core/application.h"
#include "xaloc/core/input/input.h"
#include "xaloc/filesystem/external.h"
#include "imgui/imgui_internal.h"

using namespace Xaloc;


namespace Editor
{

static const char* s_sFilenameFilter = "Xaloc Scene\0*.xaloc\0 All files\0*.*\0";

ZEditorMenuBar::ZEditorMenuBar(ZModuleEditor* pEditor)
	: m_pEditor(pEditor)
{
}

void ZEditorMenuBar::OnImGuiRender()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Xaloc"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				ZApplication::Get()->Close();
			}
			ImGui::EndMenu();
		}

		DoMenuScene();
		DoMenuView();
		DoMenuHelp();

		ImGui::EndMenuBar();
	}
}

void ZEditorMenuBar::DoMenuScene()
{
	if (ImGui::BeginMenu("Scene"))
	{
		//if (ImGui::MenuItem("Save"))
		//{
			// TODO
		//}
		if (ImGui::MenuItem("Save as..."))
		{
			std::string filename = FileSaveDialog(s_sFilenameFilter);
			if (filename != "")
			{
				ZScene::Save(m_pEditor->m_Scene, filename);
			}
		}
		if (ImGui::MenuItem("Load..."))
		{
			std::string filename = FileOpenDialog(s_sFilenameFilter);
			if (filename != "")
			{
				m_pEditor->m_Scene.reset();
				m_pEditor->m_Scene = ZScene::Load(filename);
				m_pEditor->m_SceneHierarchyPanel->SetScene(m_pEditor->m_Scene);
			}
		}
		ImGui::EndMenu();
	}
}

void ZEditorMenuBar::DoMenuView()
{
	if (ImGui::BeginMenu("View"))
	{
		if (ImGui::MenuItem("Console"))
		{
			ZApplication::Get()->GetImGui()->ShowConsole();
		}
		if (ImGui::MenuItem("Profiler"))
		{
			ZApplication::Get()->GetImGui()->ShowProfiler();
		}
		if (ImGui::MenuItem("Render Stats"))
		{
			m_pEditor->m_ShowWindowRenderStats = true;
		}
		ImGui::EndMenu();
	}
}

void ZEditorMenuBar::DoMenuHelp()
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
