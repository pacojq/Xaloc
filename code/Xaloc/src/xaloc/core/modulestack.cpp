#include "xapch.h"
#include "xaloc/core/modulestack.h"
#include "xaloc/core/module.h"


namespace Xaloc
{

ZModuleStack::ZModuleStack()
{
}

ZModuleStack::~ZModuleStack()
{
	for (ZModule* pModule : m_aModules)
	{
		pModule->OnDetach();
		delete pModule;
	}
}

void ZModuleStack::Push(ZModule* pModule)
{
	m_aPendingPush.push_back(pModule);
}

void ZModuleStack::PushOverlay(ZModule* pOverlay)
{
	m_aPendingPushOverlay.push_back(pOverlay);
}

void ZModuleStack::Pop(ZModule* pModule)
{
	m_aPendingPop.push_back(pModule);
}

void ZModuleStack::PopOverlay(ZModule* pOverlay)
{
	m_aPendingPopOverlay.push_back(pOverlay);
}

void ZModuleStack::Flush()
{
	// TODO use sparse array with nullable items and handles

	for (ZModule* pOverlay : m_aPendingPopOverlay)
	{
		auto it = std::find(m_aModules.begin(), m_aModules.end(), pOverlay);
		if (it != m_aModules.end())
		{
			m_aModules.erase(it);
			pOverlay->OnDetach();
		}
	}

	for (ZModule* pModule : m_aPendingPop)
	{
		auto it = std::find(m_aModules.begin(), m_aModules.end(), pModule);
		if (it != m_aModules.begin() + m_nInsertIndex)
		{
			m_aModules.erase(it);
			m_nInsertIndex--;
			pModule->OnDetach();
		}
	}

	for (ZModule* pModule : m_aPendingPush)
	{
		m_aModules.emplace(m_aModules.begin() + m_nInsertIndex, pModule);
		m_nInsertIndex++;
		pModule->OnAttach();
	}

	for (ZModule* pOverlay : m_aPendingPushOverlay)
	{
		m_aModules.emplace_back(pOverlay);
		pOverlay->OnAttach();
	}

	m_aPendingPopOverlay.clear();
	m_aPendingPop.clear();
	m_aPendingPush.clear();
	m_aPendingPushOverlay.clear();
}

}
