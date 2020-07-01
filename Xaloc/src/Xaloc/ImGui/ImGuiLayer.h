#pragma once

#include "Xaloc/Core/Layer.h"

#include "Xaloc/Events/KeyEvent.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Events/ApplicationEvent.h"

#include "Modules/ImGuiConsole.h"
#include "Modules/ImGuiProfiler.h"

namespace Xaloc {

	/*
		Specific layer for ImGui rendering.
	*/
	class XALOC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		virtual void OnImGuiRender() override;

		void Begin();
		void End();

		void SetBlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }


		const Ref<ImGuiProfiler>& GetProfiler() const { return m_Profiler; }

		inline void ShowConsole() { m_ShowConsole = true; }
		inline void ShowProfiler() { m_ShowProfiler = true; }
		

	private:
		bool m_BlockEvents = false;

		Ref<ImGuiProfiler> m_Profiler;
		
		bool m_ShowConsole = true;
		bool m_ShowProfiler = true; // TODO debug, change to false by default
	};

}