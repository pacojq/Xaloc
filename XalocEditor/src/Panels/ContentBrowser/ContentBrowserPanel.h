#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Assets/AssetManager.h"
#include "Xaloc/Core/Assets/AssetType.h"
#include "Xaloc/Files/FileInfo.h"

#include "Xaloc/Renderer/Texture.h"

#include "ContentBrowserHeader.h"

#include <map>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace Xaloc {

	class EditorLayer;

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel(EditorLayer* editorLayer);
		
		void OnImGuiRender();

	private:

		void RenderHierarchy();

		void RenderHierarchyRoot(const DirectoryInfo& dir);
		ImRect RenderHierarchyDir(const DirectoryInfo& dir);
		ImRect RenderHierarchyItem(const FileInfo& file);
		void OnContextMenuOpen(const FileInfo& file);


		
		void RenderFileListView(const FileInfo& file);
		void RenderFileGridView(const FileInfo& file);

		void RenderDirListView(const DirectoryInfo& dir);
		void RenderDirGridView(const DirectoryInfo& dir);

		void RenderBottom();
		
	private:
		EditorLayer* m_EditorLayer;

		Scope<ContentBrowserHeader> m_Header;

	private:		
		Ref<Texture2D> m_TexGridView;
		Ref<Texture2D> m_TexListView;
		Ref<Texture2D> m_TexSearch;
		Ref<Texture2D> m_TexButtonBack;
		Ref<Texture2D> m_TexButtonForward;

		std::string m_CurrentDirPath;
		std::string m_BaseDirPath;
		std::string m_PrevDirPath;
		std::string m_MovePath;

		int m_BasePathLen;
		bool m_IsDragging;
		bool m_IsInListView;
		bool m_UpdateBreadCrumbs;

		DirectoryInfo m_CurrentDir;
		DirectoryInfo m_BaseProjectDir;

	friend class ContentBrowserHeader;

	};
}
