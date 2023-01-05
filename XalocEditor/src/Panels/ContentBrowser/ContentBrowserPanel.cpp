#include "ContentBrowserPanel.h"

#include "./../../EditorLayer.h"

#include "Xaloc/Core/Application.h"

#include "Xaloc/Files/FileSystem.h"

#include "Xaloc/ImGui/XalocImGui.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Xaloc
{
	ContentBrowserPanel::ContentBrowserPanel(EditorLayer* editorLayer)
		: m_EditorLayer(editorLayer)
	{		
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

		m_Header = CreateScope<ContentBrowserHeader>(this);
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Project");
		{
			const float panelWidth = ImGui::GetWindowWidth();
			const float folderExplorerWidth = glm::clamp(panelWidth * .3f, 80.0f, 320.0f);

			//ImGui::Columns(2, "AB", true);
			ImGui::BeginColumns("AB", 2, ImGuiColumnsFlags_NoResize);

			//ImGui::SetColumnOffset(1, folderExplorerWidth);

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
						MAG_CORE_INFO("Moved File: " + file + " to " + m_MovePath);
						m_CurrentDir = AssetManager::ReadDirectory(m_CurrentDirPath);
					}
					*/
					m_IsDragging = false;
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::NextColumn();

			ImGui::BeginChild("##directory_structure", ImVec2(ImGui::GetColumnWidth() - 12, 0));
			{
				m_Header->OnImGuiRender();

				ImGui::Separator();

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
						MAG_CORE_INFO("Moved File: " + a + " to " + m_MovePath);
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
						std::string filename = FileSystem::OpenFileDialog();
						std::vector<std::string> outData;

						//AssetManager::ProcessAseets(filename);
					}

					if (ImGui::MenuItem("Refresh", "Ctrl + R"))
					{
						/*
						auto data = AssetManager::GetFsContents();
						for (int i = 0; i < data.size(); i++)
						{
							MAG_CORE_INFO(data[i].FileName);
						}
						*/
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImGui::EndColumns();
		}
		ImGui::End();
	}



	void ContentBrowserPanel::RenderHierarchy()
	{
		if (ImGui::CollapsingHeader("res://", nullptr, ImGuiTreeNodeFlags_DefaultOpen))
		{
			RenderHierarchyRoot(m_BaseProjectDir);
			/*
			if (ImGui::TreeNode("Contents"))
			{
				RenderHierarchyDir(m_BaseProjectDir);
				ImGui::TreePop();
			}
			*/

			if (ImGui::IsMouseDown(1))
			{
				ImGui::OpenPopup("window");
			}
		}

		ImGui::EndChild();
	}



	void ContentBrowserPanel::RenderHierarchyRoot(const DirectoryInfo& dir)
	{
		auto content = AssetManager::ReadDirectory(dir);
		const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

		ImDrawList* drawList = nullptr;
		ImVec2 verticalLineStart, verticalLineEnd;

		if (open)
		{
			const ImColor TreeLineColor = ImGui::GetColorU32(ImGuiCol_Text);
			const float SmallOffsetX = 11.0f; //for now, a hardcoded value; should take into account tree indent size
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			verticalLineStart = ImGui::GetCursorScreenPos();
			verticalLineStart.x += SmallOffsetX; //to nicely line up with the arrow symbol
			verticalLineEnd = verticalLineStart;
		}

		for (int i = 0; i < content.Directories.size(); i++)
		{
			RenderHierarchyDir(content.Directories[i]);
		}

		for (int i = 0; i < content.Files.size(); i++)
		{
			RenderHierarchyItem(content.Files[i]);
		}
	}


	ImRect ContentBrowserPanel::RenderHierarchyDir(const DirectoryInfo& dir)
	{
		bool open = XalocImGui::ImageTreeNode(dir.Name.c_str(), m_EditorLayer->GetIcons()->IconFolder(), 2.0f); //bool open = ImGui::TreeNode(dir.Name.c_str());
		const ImRect nodeRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());

		if (ImGui::BeginDragDropTarget())
		{
			auto data = ImGui::AcceptDragDropPayload("selectable");
			if (data)
			{
				/* TODO
				std::string a = (char*)data->Data;
				if (AssetManager::MoveFile(a, m_MovePath))
				{
					MAG_CORE_INFO("Moved File: " + a + " to " + m_MovePath);
				}
				*/
			}

			ImGui::EndDragDropTarget();
		}

		if (open)
		{
			auto content = AssetManager::ReadDirectory(dir);

			const ImColor TreeLineColor = ImGui::GetColorU32(ImGuiCol_Text);
			const float SmallOffsetX = -12.0f; //for now, a hardcoded value; should take into account tree indent size
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 verticalLineStart = ImGui::GetCursorScreenPos();
			verticalLineStart.x += SmallOffsetX; //to nicely line up with the arrow symbol
			ImVec2 verticalLineEnd = verticalLineStart;

			for (int i = 0; i < content.Directories.size(); i++)
			{
				const float HorizontalTreeLineSize = 8.0f; //chosen arbitrarily

				const ImRect childRect = RenderHierarchyDir(content.Directories[i]);

				const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
				drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
				verticalLineEnd.y = midpoint;
			}

			for (int i = 0; i < content.Files.size(); i++)
			{
				const float HorizontalTreeLineSize = 8.0f; //chosen arbitrarily

				const ImRect childRect = RenderHierarchyItem(content.Files[i]);

				const float midpoint = (childRect.Min.y + childRect.Max.y) / 2.0f;
				drawList->AddLine(ImVec2(verticalLineStart.x, midpoint), ImVec2(verticalLineStart.x + HorizontalTreeLineSize, midpoint), TreeLineColor);
				verticalLineEnd.y = midpoint;
			}

			drawList->AddLine(verticalLineStart, verticalLineEnd, TreeLineColor);
			
			ImGui::TreePop();
		}

		// TODO: handle stuff being dragged to this directory
		if (m_IsDragging && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		{
			// TODO m_MovePath = m_BaseProjectDir[i].AbsolutePath.c_str();
		}

		return nodeRect;
	}


	ImRect ContentBrowserPanel::RenderHierarchyItem(const FileInfo& file)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();

		ImVec2 pos = window->DC.CursorPos;
		ImRect nodeRect(pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2));


		auto iconRef = m_EditorLayer->GetIcons()->GetAssetIcon(file.Type)->GetRendererID();

		ImGui::Image((void*)iconRef, ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::SameLine();

		if (ImGui::Selectable(file.Name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick))
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// TODO: on double click open asset editor
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

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
		if (ImGui::BeginPopupContextItem(file.Name.c_str()))
		{
			OnContextMenuOpen(file);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();

		return nodeRect;
	}


	void ContentBrowserPanel::OnContextMenuOpen(const FileInfo& file)
	{
		ImGui::BeginDisabled();
		if (ImGui::MenuItem("Reload"))
		{
			// TODO reload
		}
		if (ImGui::MenuItem("Rename"))
		{
			// TODO rename
		}
		if (ImGui::MenuItem("Copy"))
		{
			// TODO copy
		}
		if (ImGui::MenuItem("Duplicate"))
		{
			// TODO duplicate
		}
		if (ImGui::MenuItem("Delete"))
		{
			// TODO delete
		}
		ImGui::EndDisabled();

		ImGui::Separator();

		ImGui::BeginDisabled();
		if (ImGui::MenuItem("Show In Explorer"))
		{
			// TODO
		}
		ImGui::EndDisabled();
		if (ImGui::MenuItem("Open Externally"))
		{
			FileSystem::OpenExternally(file.AbsolutePath);
		}
	}
	





	void ContentBrowserPanel::RenderFileListView(const FileInfo& file)
	{
		//auto fileType = m_CurrentDir[dirIndex].FileType;
		auto iconRef = m_EditorLayer->GetIcons()->GetAssetIcon(file.Type)->GetRendererID();

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

	
	void ContentBrowserPanel::RenderFileGridView(const FileInfo& file)
	{
		ImGui::BeginGroup();

		//auto fileID = AssetTypes::GetParsedAssetID(file.Type);
		auto iconRef = m_EditorLayer->GetIcons()->GetAssetIcon(file.Type)->GetRendererID();

		ImGui::ImageButton((void*)iconRef, ImVec2(70, 70), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		auto fname = file.Name;
		//auto newFname = AssetManager::StripExtras(fname);

		ImGui::TextWrapped(fname.c_str());
		ImGui::EndGroup();

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

	void ContentBrowserPanel::RenderDirListView(const DirectoryInfo& dir)
	{
		ImGui::Image((void*)m_EditorLayer->GetIcons()->IconFolder()->GetRendererID(), ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
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
			ImGui::Image((void*)m_EditorLayer->GetIcons()->IconFolder()->GetRendererID(), ImVec2(20, 20), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::SameLine();
			ImGui::Text(dir.Name.c_str());
			int size = sizeof(const char*) + strlen(dir.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", dir.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
	}

	void ContentBrowserPanel::RenderDirGridView(const DirectoryInfo& dir)
	{
		ImGui::BeginGroup();
		ImGui::ImageButton((void*)m_EditorLayer->GetIcons()->IconFolder()->GetRendererID(), ImVec2(70, 70), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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
			ImGui::Image((void*)m_IconFolder->GetRendererID(), ImVec2(20, 20));
			ImGui::SameLine();
			ImGui::Text(dir.Name.c_str());
			int size = sizeof(const char*) + strlen(dir.AbsolutePath.c_str());
			ImGui::SetDragDropPayload("selectable", dir.AbsolutePath.c_str(), size);
			m_IsDragging = true;
			ImGui::EndDragDropSource();
		}
		*/
	}


	void ContentBrowserPanel::RenderBottom()
	{
		ImGui::BeginChild("##nav", ImVec2(ImGui::GetColumnWidth() - 12, 23));
		{
			ImGui::EndChild();
		}
	}
}

