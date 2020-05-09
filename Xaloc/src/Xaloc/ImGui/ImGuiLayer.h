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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		void RenderFPS();

		
	private:
		float m_Time = 0.0f;

		float m_FpsValues[50];
		std::vector<float> m_FrameTimes;
	};

}