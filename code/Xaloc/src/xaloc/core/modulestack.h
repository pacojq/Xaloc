#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/module.h"
#include <vector>


namespace Xaloc
{

/*
	TODO: rework this.
		- use a handle class and have ZModuleStack create and destroy modules
		- clean up "pending" queues
*/

class ZModuleStack
{
public:
	ZModuleStack();
	~ZModuleStack();

	void Push(ZModule* pModule);
	void PushOverlay(ZModule* pOverlay);

	void Pop(ZModule* pModule);
	void PopOverlay(ZModule* pOverlay);

	/// <summary>
	/// Pushes and pops the pending modules.
	/// </summary>
	void Flush();

	std::vector<ZModule*>::iterator begin() { return m_aModules.begin(); }
	std::vector<ZModule*>::iterator end() { return m_aModules.end(); }

private:
	std::vector<ZModule*> m_aModules;

	std::vector<ZModule*> m_aPendingPush;
	std::vector<ZModule*> m_aPendingPushOverlay;
	std::vector<ZModule*> m_aPendingPop;
	std::vector<ZModule*> m_aPendingPopOverlay;

	uint32 m_nInsertIndex = 0;
};


}
