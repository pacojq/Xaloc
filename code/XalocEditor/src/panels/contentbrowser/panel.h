#pragma once
#include "xaloc/core/core.h"
#include "xaloc/filesystem/info.h"
#include "xaloc/renderer/resource/texture.h"
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <string>
#include <map>

namespace Editor {
	class ZContentBrowserHeader;
	class ZModuleEditor;
}

using namespace Xaloc;


namespace Editor
{

class ZPanelContentBrowser
{
public:
	ZPanelContentBrowser(ZModuleEditor* pEditor);
	~ZPanelContentBrowser();

	void OnImGuiRender();

private:

	void RenderHierarchy();

	void RenderHierarchyRoot(const SDirectoryInfo& dir);
	ImRect RenderHierarchyDir(const SDirectoryInfo& dir);
	ImRect RenderHierarchyItem(const SFileInfo& file);
	void OnContextMenuOpen(const SFileInfo& file);

	void RenderFileListView(const SFileInfo& file);
	void RenderFileGridView(const SFileInfo& file);

	void RenderDirListView(const SDirectoryInfo& dir);
	void RenderDirGridView(const SDirectoryInfo& dir);

	void RenderBottom();

private:
	ZModuleEditor* m_pEditor;
	ZContentBrowserHeader* m_Header;

private:
	ZRef<ZTexture2D> m_TexGridView;
	ZRef<ZTexture2D> m_TexListView;
	ZRef<ZTexture2D> m_TexSearch;
	ZRef<ZTexture2D> m_TexButtonBack;
	ZRef<ZTexture2D> m_TexButtonForward;

	std::string m_CurrentDirPath;
	std::string m_BaseDirPath;
	std::string m_PrevDirPath;
	std::string m_MovePath;

	int m_BasePathLen;
	bool m_IsDragging;
	bool m_IsInListView;
	bool m_UpdateBreadCrumbs;

	SDirectoryInfo m_CurrentDir;
	SDirectoryInfo m_BaseProjectDir;

	// TODO: get rid of friend class
	friend class ZContentBrowserHeader;

};

}
