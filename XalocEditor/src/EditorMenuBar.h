#pragma once

#include "Xaloc.h"

namespace Xaloc {

	class EditorLayer;

	class EditorMenuBar
	{
	public:
		EditorMenuBar(EditorLayer* editorLayer);

		void OnImGuiRender();

	private:
		void DoMenuScene();
		void DoMenuView();
		void DoMenuHelp();


	private:
		EditorLayer* m_EditorLayer;


		static inline std::string s_FilenameFilter = "Xaloc Scene\0*.xaloc\0 All files\0*.*\0";
	};

}
