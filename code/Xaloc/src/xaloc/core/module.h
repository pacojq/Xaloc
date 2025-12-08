#pragma once
#include "xaloc/core/core.h"
#include "xaloc/core/time.h"
#include "xaloc/events/event.h"


namespace Xaloc
{

/*
	Xaloc is a module-based engine.
	Modules can be attached to and detached from the
	main loop and execute OnUpdate and OnEvent functions.
*/
class ZModule
{
public:
	ZModule(const std::string& sName);
	virtual ~ZModule() = default;

	virtual void OnAttach() {}
	virtual void OnDetach() {}
	virtual void OnUpdate(ZTimestep ts) {}
	virtual void OnImGuiRender() {}
	virtual void OnEvent(ZEvent& rEvent) {}

	inline const std::string& GetName() const { return m_sName; }

protected:
	std::string m_sName;
};

}
