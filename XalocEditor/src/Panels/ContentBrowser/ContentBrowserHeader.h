#pragma once

#include "Xaloc/Core/Core.h"

namespace Xaloc
{
	class ContentBrowserPanel;

	class ContentBrowserHeader
	{
	public:
		ContentBrowserHeader(ContentBrowserPanel* panel);

		void OnImGuiRender();

	private:
		ContentBrowserPanel* m_Panel;

		bool m_ShowSearchBar;

		char* m_InputText;
		char* m_InputHint;
		char  m_InputBuffer[1024];
	};
}
