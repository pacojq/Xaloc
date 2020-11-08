#include "AssetManagerPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Xaloc/Core/Application.h"

namespace Xaloc
{
	AssetManagerPanel::AssetManagerPanel()
	{
		m_TexFolder = Texture2D::Create("assets/editor/folder.png");
		m_TexFile = Texture2D::Create("assets/editor/file.png");
		
		m_TexGridView = Texture2D::Create("assets/editor/grid.png");
		m_TexListView = Texture2D::Create("assets/editor/list.png");
		m_TexSearch = Texture2D::Create("assets/editor/search.png");
		m_TexButtonBack = Texture2D::Create("assets/editor/btn-back.png");
		m_TexButtonForward = Texture2D::Create("assets/editor/btn-forward.png");

		
		m_BaseDirPath = "assets";
		m_CurrentDirPath = m_BaseDirPath;
		m_PrevDirPath = m_CurrentDirPath;
		m_BaseProjectDir = AssetManager::GetRoot();
		
		m_CurrentDir = m_BaseProjectDir;
		m_BasePathLen = strlen(m_BaseDirPath.c_str());

		m_IsDragging = false;
		m_IsInListView = true;
		m_UpdateBreadCrumbs = true;
		m_ShowSearchBar = false;


		m_AssetIconMap[AssetType::File] = m_TexFile;

		m_AssetIconMap[AssetType::Text] = m_TexFile;
		m_AssetIconMap[AssetType::Image] = m_TexFile;
		m_AssetIconMap[AssetType::Audio] = m_TexFile;
		m_AssetIconMap[AssetType::Shader] = m_TexFile;
		m_AssetIconMap[AssetType::Material] = m_TexFile;
		m_AssetIconMap[AssetType::Scene] = m_TexFile;
		m_AssetIconMap[AssetType::Code] = m_TexFile;
		m_AssetIconMap[AssetType::Dll] = m_TexFile;
		m_AssetIconMap[AssetType::Font] = m_TexFile;
		
		m_InputHint = "Search...";
		m_InputText = "";
	}

	void AssetManagerPanel::OnImGuiRender()
	{
		ImGui::Begin("Project");
		{
			ImGui::Columns(2, "AB", true);
			ImGui::SetColumnOffset(1, 240);

			ImGui::BeginChild("##folders_common");
			{
				RenderHierarchy();
			}

			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("selectable", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
				if (data)
				{
					/* TODO
					std::string file = (char*)data->Data;
					if (AssetManager::MoveFile(file, m_MovePath))
					{
						XA_CORE_INFO("Moved File: " + file + " to " + m_MovePath);
						m_CurrentDir = AssetManager::ReadDirectory(m_CurrentDirPath);
					}
					*/
					m_IsDragging = false;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();

			ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetColumnWidth() - 12, 250));
			{
				RenderBreadCrumbs();
				ImGui::SameLine();
				ImGui::Dummy(ImVec2(ImGui::GetColumnWidth() - 350, 0));
				ImGui::SameLine();
				RenderSearch();
				ImGui::EndChild();

				ImGui::BeginChild("Scrolling");
				if (!m_IsInListView)
					ImGui::Columns(17, nullptr, false);

				auto content = AssetManager::ReadDirectory(m_CurrentDir);

				for (int i = 0; i < content.Directories.size(); i++)
				{
					auto dir = content.Directories[i];
					if (!m_IsInListView) RenderDirGridView(dir);
					else                 RenderDirListView(dir);
					ImGui::NextColumn();
				}
				
				for (int i = 0; i < content.Files.size(); i++)
				{
					auto file = content.Files[i];
					if (!m_IsInListView) RenderFileGridView(file);
					else                 RenderFileListView(file);
					ImGui::NextColumn();
				}

				ImGui::EndChild();
				ImGui::EndChild();
			}

			
			if (ImGui::BeginDragDropTarget())
			{
				auto data = ImGui::AcceptDragDropPayload("selectable", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);
				if (data)
				{
					/* TODO
					std::string a = (char*)data->Data;
					if (AssetManager::MoveFile(a, m_MovePath))
					{
						XA_CORE_INFO("Moved File: " + a + " to " + m_MovePath);
					}
					*/
					m_IsDragging = false;
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Import New Asset", "Ctrl + O"))
					{
						std::string filename = Application::Get().OpenFile("");
						std::vector<std::string> outData;

						//AssetManager::ProcessAseets(filename);
					}

					if (ImGui::MenuItem("Refresh", "Ctrl + R"))
					{
						/*
						auto data = AssetManager::GetFsContents();
						for (int i = 0; i < data.size(); i++)
						{
							XA_CORE_INFO(data[i].FileName);
						}
						*/
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::End();
		}
	}



	void AssetManagerPanel::RenderHierarchy()
	{
		if (ImGui::CollapsingHeader("res://", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::TreeNode("Contents"))
			{
				RenderHierarchyDir(m_BaseProjectDir);
				ImGui::TreePop();
			}

			if (ImGui::IsMouseDown(1))
			{
				ImGui::OpenPopup("window");
			}
		}

		ImGui::EndChild();
	}

	void AssetManagerPanel::RenderHierarchyDir(const DirectoryInfo& dir)
	{
		auto content = AssetManager::ReadDirectory(dir);

		for (int i = 0; i < content.Directories.size(); i++)
		{
			if (ImGui::TreeNode(content.Directories[i].Name.c_str()))
			{
				RenderHierarchyDir(content.Directories[i]);
				ImGui::TreePop();
			}

			if (m_IsDragging && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				// TODO m_MovePath = m_BaseProjectDir[i].AbsolutePath.c_str();
			}
		}

		for (int i = 0; i < content.Files.size(); i++)
		{
			ImGui::Selectable(content.Files[i].Name.c_str(), false);
		}
	}





	void AssetManagerPanel::RenderBreadCrumbs()
	{
		ImGui::BeginChild("##directory_breadcrumbs", ImVec2(ImGui::GetColumnWidth() - 100, 30));
		{
			if (m_IsInListView)
			{
				if (ImGui::ImageButton((void*)m_TexGridView->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
				{
					m_IsInListView = !m_IsInListView;
				}
				ImGui::SameLine();

			}
			else
			{
				if (ImGui::ImageButton((void*)m_TexListView->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
				{
					m_IsInListView = !m_IsInListView;
				}
				ImGui::SameLine();

			}

			if (ImGui::ImageButton((void*)m_TexSearch->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
			{
				m_ShowSearchBar = !m_ShowSearchBar;
			}
			ImGui::SameLine();

			if (m_ShowSearchBar)
			{
				char buff[100] = { 0 };
				ImGui::SameLine();
				ImGui::PushItemWidth(200);
				ImGui::InputTextWithHint(m_InputText, m_InputHint, buff, 100);
				ImGui::PopItemWidth();
				ImGui::SameLine();
			}
			
			if (ImGui::ImageButton((void*)m_TexButtonBack->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
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
			if (ImGui::ImageButton((void*)m_TexButtonForward->GetRendererID(), ImVec2(20, 18), ImVec2{ 0, 1 }, ImVec2{ 1, 0 }))
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
	}


	

	void AssetManagerPanel::RenderFileListView(const FileInfo& file)
	{
		//auto fileType = m_CurrentDir[dirIndex].FileType;
		auto iconRef = m_AssetIconMap[file.Type]->GetRendererID();

		ImGui::Image((void*)iconRef, ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::SameLine();
		if (ImGui::Selectable(file.Name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				std::string cmd = '"' + file.AbsolutePath + '"';
				system(cmd.c_str());
			}
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Image((void*)iconRef, ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::SameLine();
			ImGui::Text(file.Name.c_str());
			int size = sizeof(const char*) + strlen(file.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", file.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
	}

	
	void AssetManagerPanel::RenderFileGridView(const FileInfo& file)
	{
		ImGui::BeginGroup();

		//auto fileID = AssetTypes::GetParsedAssetID(file.Type);
		auto iconRef = m_AssetIconMap[file.Type]->GetRendererID();

		ImGui::ImageButton((void*)iconRef, ImVec2(70, 70), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		auto fname = file.Name;
		//auto newFname = AssetManager::StripExtras(fname);

		ImGui::TextWrapped(fname.c_str());
		ImGui::EndGroup();

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::Image((void*)m_TexFile->GetRendererID(), ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::SameLine();

			ImGui::Text(file.Name.c_str());
			int size = sizeof(const char*) + strlen(file.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", file.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
	}

	void AssetManagerPanel::RenderDirListView(const DirectoryInfo& dir)
	{
		ImGui::Image((void*)m_TexFolder->GetRendererID(), ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::SameLine();

		if (ImGui::Selectable(dir.Name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				m_PrevDirPath = dir.AbsolutePath;
				m_CurrentDirPath = dir.AbsolutePath;
				m_CurrentDir = dir;
			}
		}

		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
		{
			ImGui::Image((void*)m_TexFolder->GetRendererID(), ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::SameLine();
			ImGui::Text(dir.Name.c_str());
			int size = sizeof(const char*) + strlen(dir.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", dir.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
	}

	void AssetManagerPanel::RenderDirGridView(const DirectoryInfo& dir)
	{
		ImGui::BeginGroup();
		ImGui::ImageButton((void*)m_TexFolder->GetRendererID(), ImVec2(70, 70), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::IsMouseDoubleClicked(0))
		{
			m_PrevDirPath = dir.AbsolutePath;
			m_CurrentDirPath = dir.AbsolutePath;
			m_CurrentDir = dir;
		}

		auto fname = dir.Name;
		//auto newFname = AssetManager::StripExtras(fname);
		ImGui::TextWrapped(fname.c_str());
		ImGui::EndGroup();

		/*
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
		{
			ImGui::Image((void*)m_TexFolder->GetRendererID(), ImVec2(20, 20));
			ImGui::SameLine();
			ImGui::Text(dir.Name.c_str());
			int size = sizeof(const char*) + strlen(dir.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", dir.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
		*/
	}

	void AssetManagerPanel::RenderSearch()
	{
		ImGui::BeginChild("##search_menu", ImVec2(320, 30));
		{
			char buff[100] = { 0 };
			ImGui::Image((void*)m_TexSearch->GetRendererID(), ImVec2(22, 22), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::SameLine();
			ImGui::InputTextWithHint(m_InputText, m_InputHint, buff, 100);
		}
		ImGui::EndChild();
	}

	void AssetManagerPanel::RenderBottom()
	{
		ImGui::BeginChild("##nav", ImVec2(ImGui::GetColumnWidth() - 12, 23));
		{
			ImGui::EndChild();
		}
	}
}

