#pragma once

#include "Xaloc/Core/Layer.h"

#include "Xaloc/Events/KeyEvent.h"
#include "Xaloc/Events/MouseEvent.h"
#include "Xaloc/Events/ApplicationEvent.h"

#include "Modules/ImGuiConsole.h"

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

	private:
		void RenderFPS();

		
	private:
		bool m_BlockEvents = false;
		float m_Time = 0.0f;

		float m_FpsValues[50];
		std::vector<float> m_FrameTimes;
	};

}