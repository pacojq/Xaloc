#include "xapch.h"
#include "Application.h"

#include "Xaloc/Core/Log.h"
#include "Xaloc/Core/Assets/AssetManager.h"

#include "Xaloc/ImGui/ImGuiStyles.h"

#include "Xaloc/Renderer/Renderer.h"

#include "Xaloc/Scripting/ScriptEngine.h"


#include <GLFW/glfw3.h>


namespace Xaloc {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;


	Application::Application(ApplicationSpec spec)
		: m_Name(spec.Name)
	{
		XA_PROFILE_FUNCTION();
		
		XA_CORE_ASSERT(!s_Instance, "Application should be null");
		s_Instance = this;

		m_PauseOnFocusLost = spec.PauseOnFocusLost;

		AssetManager::Init(spec.AssetsRootPath);
		
		RendererAPI::SetAPI(spec.TargetGraphics);

		WindowProps props;
		props.Title = spec.Name;
		props.Width = spec.Width;
		props.Height = spec.Height;
		props.IsVSyncEnabled = spec.VSyncEnabled;
		
		m_Window = std::unique_ptr<Window>(Window::Create(props));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		m_LayerStack.Flush();

		ImGuiStyles::ApplyDefaultStyle();

		if (spec.UseScripting)
		{
			// TODO error handling with ScriptingDllPath
			ScriptEngine::Init(spec.ScriptingDllPath);
		}
	}

	Application::~Application()
	{
		XA_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}


	void Application::PushLayer(Layer* layer)
	{
		XA_PROFILE_FUNCTION();
		
		m_LayerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		XA_PROFILE_FUNCTION();

		m_LayerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		XA_PROFILE_FUNCTION();
		
		m_LayerStack.PushOverlay(overlay);
	}


	void Application::OnEvent(Event& e)
	{
		XA_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FN(OnWindowFocus));
		
		//XA_CORE_TRACE("{0}", e);

		// Dispatch events down to our layer stack
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			if (e.Handled)
				break;
			(*--it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		XA_PROFILE_FUNCTION();

		m_LayerStack.Flush();
		
		while (m_Running)
		{
			XA_PROFILE_SCOPE("RunLoop");
			
			float time = (float) glfwGetTime(); // TODO Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_FPS = 1.0f / timestep;
			m_Timestep = timestep * 1000.0f; // seconds to milliseconds

			if (!m_Minimized)
			{
				if (!m_Paused)
				{
					m_ImGuiLayer->GetProfiler()->BeginFrame();
					
					XA_PROFILE_SCOPE("LayerStack OnUpdate");
					
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);

					m_ImGuiLayer->GetProfiler()->EndFrame();
				}

				if (m_ImGuiEnabled)
				{
					m_ImGuiLayer->Begin();
					{
						XA_PROFILE_SCOPE("LayerStack OnImGuiRender");

						for (Layer* layer : m_LayerStack)
							layer->OnImGuiRender();
					}
					m_ImGuiLayer->End();
				}

				m_LayerStack.Flush();
			}

			m_Window->OnUpdate();
		}

		// TODO Shutdown
		ScriptEngine::Shutdown();
	}

	void Application::Close()
	{
		m_Running = false;
	}


	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		XA_PROFILE_FUNCTION();
		
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());


		return true;
	}

	bool Application::OnWindowFocus(WindowFocusEvent& e)
	{
		if (e.IsFocused())
			XA_CORE_INFO("Window focus gain registered.");
		else XA_CORE_INFO("Window focus loss registered.");

		if (m_PauseOnFocusLost)
		{
			if (e.IsFocused() && !m_Focused)
				m_Paused = false;
			
			else if (!e.IsFocused() && m_Focused)
				m_Paused = true;
		}

		m_Focused = e.IsFocused();
		
		return false;
	}


}
