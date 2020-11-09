#pragma once

#include "Core.h"

#include "Window.h"

#include "Xaloc/Renderer/Renderer.h"

#include "Xaloc/Core/LayerStack.h"
#include "Xaloc/Events/Event.h"
#include "Xaloc/Events/ApplicationEvent.h"

#include "Xaloc/Core/Timestep.h"

#include "Xaloc/ImGui/ImGuiLayer.h"


namespace Xaloc {


	struct ApplicationSpec
	{
		std::string Name = "Xaloc App";

		/// <summary>
		/// Whether the application will use a C# scripting project.
		/// </summary>
		bool UseScripting = false;

		/// <summary>
		/// The path to the C# scripting project DLL.
		/// This will be required if UseScripting is true.
		/// </summary>
		std::string ScriptingDllPath;

		RendererAPI::API TargetGraphics = RendererAPI::API::OpenGL;

		/// <summary>
		/// Default width of the application display.
		/// </summary>
		uint32_t Width = 1280;

		/// <summary>
		/// Default height of the application display.
		/// </summary>
		uint32_t Height = 720;

		/// <summary>
		/// Whether the game should pause its execution
		/// when the application window loses focus..
		/// </summary>
		bool PauseOnFocusLost = false;

		bool VSyncEnabled = true;

		std::string AssetsRootPath = "assets";
	};
	

	class Application
	{
	public:
		Application(ApplicationSpec spec);
		virtual ~Application();

		void Run();
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);


		// TODO move somewhere else
		std::string OpenFile(const std::string& filter) const;
		// TODO move somewhere else
		std::string SaveFile(const std::string& filter) const;


		
		inline Window& GetWindow() { return *m_Window; }

		inline void SetImGuiEnabled(bool enabled) { s_Instance->m_ImGuiEnabled = enabled; }
		inline bool IsImGuiEnabled() { return s_Instance->m_ImGuiEnabled; }

		inline ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
		
		

		
		/// <returns>The Application singleton.</returns>
		inline static Application& Get() { return *s_Instance; }

		inline static std::string GetName() { return s_Instance->m_Name; }

		inline static float GetFPS() { return s_Instance->m_FPS; }
		inline static float GetFrameTime() { return s_Instance->m_Timestep; }

		inline static void SetPaused(bool paused) { s_Instance->m_Paused = paused; }
		inline static bool IsPaused() { return s_Instance->m_Paused; }

		
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowFocus(WindowFocusEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		std::string m_Name;

		/// <summary>
		/// Whether the application is running.
		/// </summary>
		bool m_Running = true;

		/// <summary>
		/// Whether the game is running in a "freeze" state (might be true
		/// if the application window focus is lost).
		/// </summary>
		bool m_Paused = false;

		/// <summary>
		/// Whether the application window is minimized.
		/// </summary>
		bool m_Minimized = false;

		/// <summary>
		/// Whether the application window has focus.
		/// </summary>
		bool m_Focused = true;

		bool m_PauseOnFocusLost;

		bool m_ImGuiEnabled = true;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		
		float m_FPS = 0.0f;
		float m_Timestep = 0.0f;


		
	private:
		/// <summary>
		/// Application singleton.
		/// </summary>
		static Application* s_Instance;
	};

	
	/// <summary>
	/// To be defined in the client code.
	/// Used at start time  to create the actual client application instance.
	/// </summary>
	/// <returns>The client application.</returns>
	Application* CreateApplication();

}

