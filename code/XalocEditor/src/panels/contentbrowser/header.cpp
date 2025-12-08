#include "panels/contentbrowser/header.h"
#include "panels/contentbrowser/panel.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

using namespace Xaloc;


namespace Editor
{

ZContentBrowserHeader::ZContentBrowserHeader(ZPanelContentBrowser* panel)
	: m_Panel(panel)
{
	m_ShowSearchBar = false;

	m_InputHint = "Search...";
	m_InputText = "";
}

void ZContentBrowserHeader::OnImGuiRender()
{
	ImGui::BeginChild("##directory_breadcrumbs", ImVec2(ImGui::GetColumnWidth() - 100, 30));
	{
		const char* sIconID = "##directory_breadcrumbs_icon";
		if (m_Panel->m_IsInListView)
		{
			if (ImGui::ImageButton(sIconID, (void*)m_Panel->m_TexGridView->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				m_Panel->m_IsInListView = !m_Panel->m_IsInListView;
			}
			ImGui::SameLine();
		}
		else
		{
			if (ImGui::ImageButton(sIconID, (void*)m_Panel->m_TexListView->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				m_Panel->m_IsInListView = !m_Panel->m_IsInListView;
			}
			ImGui::SameLine();

		}

		const char* sIconSearchID = "##directory_breadcrumbs_iconSearch";
		if (ImGui::ImageButton(sIconSearchID, (void*)m_Panel->m_TexSearch->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
		{
			m_ShowSearchBar = !m_ShowSearchBar;
		}
		ImGui::SameLine();

		if (m_ShowSearchBar)
		{
			char buff[100] = { 0 };
			ImGui::SameLine();
			ImGui::PushItemWidth(200);
			ImGui::InputTextWithHint("##search_menu_input", m_InputHint, buff, 100);
			ImGui::PopItemWidth();
			ImGui::SameLine();
		}

		const char* sIconBackID = "##directory_breadcrumbs_iconBack";
		if (ImGui::ImageButton(sIconBackID, (void*)m_Panel->m_TexButtonBack->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
		{
			/* TODO
			if (strlen(m_CurrentDirPath.c_str()) != m_basePathLen)
			{
				m_PrevDirPath = AssetManager::GetParentPath(m_CurrentDirPath);
				m_CurrentDirPath = m_PrevDirPath;
				m_CurrentDir = AssetManager::ReadDirectory(m_CurrentDirPath);
			}
			*/
		}
		ImGui::SameLine();
		const char* sIconForwardID = "##directory_breadcrumbs_iconForward";
		if (ImGui::ImageButton(sIconForwardID, (void*)m_Panel->m_TexButtonForward->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
		{
			/* TODO
			m_PrevDirPath = AssetManager::GetParentPath(m_CurrentDirPath);
			m_CurrentDirPath = m_lastNavPath;
			m_CurrentDir = AssetManager::ReadDirectory(m_lastNavPath);
			*/
		}
		ImGui::SameLine();

		/*
		auto data = AssetManager::GetDirectories(m_CurrentDirPath);

		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] != m_BaseDirPath) {
				ImGui::Image((void*)m_folderRightTex->GetRendererID(), ImVec2(22, 23));
			}
			ImGui::SameLine();
			ImGui::Text(data[i].c_str());
			ImGui::SameLine();
		}

		ImGui::SameLine();
		*/

		ImGui::Dummy(ImVec2(ImGui::GetColumnWidth() - 400, 0));

		ImGui::SameLine();
	}
	ImGui::SameLine();
	ImGui::Dummy(ImVec2(ImGui::GetColumnWidth() - 350, 0));
	ImGui::EndChild();
}

}

