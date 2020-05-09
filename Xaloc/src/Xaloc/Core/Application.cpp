#include "xapch.h"
#include "Application.h"

#include "Xaloc/Core/Log.h"

#include "Xaloc/Renderer/Renderer.h"

#include "Input/Input.h"

#include <GLFW/glfw3.h>


namespace Xaloc {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application(ApplicationSpec spec)
		: m_Name(spec.Name)
	{
		XA_CORE_ASSERT(!s_Instance, "Application should be null");
		s_Instance = this;

		RendererAPI::SetAPI(spec.TargetGraphics);

		WindowProps props;
		props.Title = spec.Name;
		props.Width = spec.Width;
		props.Height = spec.Height;
		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}


	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		
		//XA_CORE_TRACE("{0}", e);

		// Dispatch events down to our layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float) glfwGetTime(); // TODO Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_FPS = 1.0f / timestep;

			if (!m_Minimized && !m_Paused)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}


			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());


		return true;
	}

}