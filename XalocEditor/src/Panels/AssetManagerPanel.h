#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Assets/AssetManager.h"
#include "Xaloc/Core/Assets/FileInfo.h"

#include "Xaloc/Renderer/Texture.h"

#include <map>

namespace Xaloc
{
	class AssetManagerPanel
	{
	public:
		AssetManagerPanel();
		
		void OnImGuiRender();

	private:

		void RenderHierarchy();
		void RenderHierarchyDir(const DirectoryInfo& dir);
		
		void RenderFileListView(const FileInfo& file);
		void RenderFileGridView(const FileInfo& file);

		void RenderDirListView(const DirectoryInfo& dir);
		void RenderDirGridView(const DirectoryInfo& dir);

		void RenderBreadCrumbs();

		void RenderSearch();

		void RenderBottom();
		
	private:
		std::map<AssetType, Ref<Texture2D>> m_AssetIconMap;
		
		Ref<Texture2D> m_TexFolder;
		Ref<Texture2D> m_TexFile;
		
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
		bool m_ShowSearchBar;

		char* m_InputText;
		char* m_InputHint;
		char  m_InputBuffer[1024];

		DirectoryInfo m_CurrentDir;
		DirectoryInfo m_BaseProjectDir;
	};
}
