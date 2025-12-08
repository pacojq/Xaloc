#pragma once
#include "xaloc/core/core.h"
#include "panels/contentbrowser/panel.h"

using namespace Xaloc;


namespace Editor
{

class ZContentBrowserHeader
{
public:
	ZContentBrowserHeader(ZPanelContentBrowser* panel);

	void OnImGuiRender();

private:
	ZPanelContentBrowser* m_Panel;

	bool m_ShowSearchBar;

	char* m_InputText;
	char* m_InputHint;
	char  m_InputBuffer[1024];
};

}
