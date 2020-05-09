#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Timestep.h"
#include "Xaloc/Events/Event.h"

namespace Xaloc {

	/*
		Xaloc is a layer-based engine.
		Layers can be attached to and detached from the
		main loop and execute OnUpdate and OnEvent functions.
	*/

	class XALOC_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}
