#pragma once

#include "Xaloc/ImGui/ImGuiLayer.h"

namespace Xaloc {

	class DirectXImGuiLayer : public ImGuiLayer
	{
	public:
		DirectXImGuiLayer();
		virtual ~DirectXImGuiLayer() = default;

		virtual void Begin() override;
		virtual void End() override;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};

}
