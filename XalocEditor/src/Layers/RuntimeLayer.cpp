#include "RuntimeLayer.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include "imgui/imgui.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#include <glm/gtc/type_ptr.hpp>


namespace Xaloc {


	RuntimeLayer::RuntimeLayer(const Ref<Scene>& scene) : Layer("Runtime Layer")
	{
		// Load Assets

		AssetManager::LoadTexture("TILEMAP", "assets/game/textures/tilemap.png");


		// Init Scene
		// TODO save current scene state
		m_Scene = scene;

		ScriptEngine::SetSceneContext(m_Scene);
		m_Scene->StartRuntime();
	}


	void RuntimeLayer::OnAttach()
	{
		FramebufferSpec framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification renderPassSpec;
		renderPassSpec.TargetFramebuffer = framebuffer;
		m_RenderPass = RenderPass::Create(renderPassSpec);
	}

	void RuntimeLayer::OnDetach()
	{
		// TODO reload state before playing
	}


	void RuntimeLayer::OnUpdate(Timestep ts)
	{
		// UPDATE AND RENDER SCENE

		Renderer::BeginRenderPass(m_RenderPass);
		m_Scene->OnUpdateRuntime(ts);
		Renderer::EndRenderPass();
	}



	void RuntimeLayer::OnImGuiRender()
	{
		// ============================================= VIEWPORTS ============================================= //
		/*
		m_GameViewport->Begin();
		if (m_GameViewport->Render(m_RenderPass))
		{
			// TODO OnWindow resize, call m_CameraController.OnResize again 
			//m_CameraController.OnResize(viewportSize.x, viewportSize.y);

			auto& data = m_MainCamera.GetComponent<CameraComponent>();
			uint32_t width = (uint32_t)m_GameViewport->GetSize().x;
			uint32_t height = (uint32_t)m_GameViewport->GetSize().y;
			data.Camera.SetViewportSize(width, height);
		}
		m_GameViewport->End();
		*/
	}

	void RuntimeLayer::OnEvent(Event& e)
	{
		m_Scene->OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(XA_BIND_EVENT_FN(RuntimeLayer::OnWindowResized));
	}





	bool RuntimeLayer::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() <= 0.0f || e.GetHeight() <= 0.0f) // Minimized
			return false;

		// TODO when dragging the scene viewport to dock it somewhere else, the camera rotates in a weird way

		return false;
	}


}
