#pragma once
#include <string>

namespace Editor {
	class ZModuleEditor;
}

using namespace Xaloc;


namespace Editor
{

class ZEditorMenuBar
{
public:
	ZEditorMenuBar(ZModuleEditor* pEditor);

	void OnImGuiRender();

private:
	void DoMenuScene();
	void DoMenuView();
	void DoMenuHelp();

private:
	ZModuleEditor* m_pEditor;
};

}
