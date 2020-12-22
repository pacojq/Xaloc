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
		To be implemented by each rendering platform.
	*/
	class ImGuiLayer : public Layer
	{
	public:
		static ImGuiLayer* Create();

		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void OnEvent(Event& e) override;
		
		void SetBlockEvents(bool blockEvents) { m_BlockEvents = blockEvents; }

		const Ref<ImGuiProfiler>& GetProfiler() const { return m_Profiler; }

		inline void ShowConsole() { m_ShowConsole = true; }
		inline void ShowProfiler() { m_ShowProfiler = true; }


	protected:
		ImGuiLayer::ImGuiLayer();

		/// <summary>
		/// To be called by children on attach layer.
		/// </summary>
		void SetupImGui();
		

	protected:
		bool m_BlockEvents = false;

		Ref<ImGuiProfiler> m_Profiler;
		
		bool m_ShowConsole = true;
		bool m_ShowProfiler = true; // TODO debug, change to false by default
	};

}