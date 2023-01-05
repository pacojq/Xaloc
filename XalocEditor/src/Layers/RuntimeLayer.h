#pragma once

#include "Xaloc.h"
#include "../Viewports/EditorGamePreview.h"

#include "../Panels/AssetManagerPanel.h"
#include "../Panels/SceneHierarchyPanel.h"

namespace Xaloc {

	class RuntimeLayer : public Layer
	{
	public:
		RuntimeLayer(const Ref<Scene>& scene);
		virtual ~RuntimeLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		
		bool OnWindowResized(WindowResizeEvent& e);

		
	private:

		Ref<Scene> m_Scene;

		Ref<RenderPass> m_RenderPass;
		
		Ref<EditorGamePreview> m_GamePreview;


	// Editor Windows
	private:
		bool m_ShowWindowConsole = true; // TODO
		bool m_ShowWindowRenderStats = true;
	};

}