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

		bool UseScripting = false;
		std::string ScriptingDllPath;

		RendererAPI::API TargetGraphics = RendererAPI::API::OpenGL;

		uint32_t Width = 1280;
		uint32_t Height = 720;

		bool PauseOnFocusLost = false;

		bool VSyncEnabled = true;
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
		void PushOverlay(Layer* overlay);


		// TODO move somewhere else
		std::string OpenFile(const std::string& filter) const;
		// TODO move somewhere else
		std::string SaveFile(const std::string& filter) const;


		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		inline ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }

		inline static std::string GetName() { return s_Instance->m_Name; }

		inline static float GetFPS() { return s_Instance->m_FPS; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowFocus(WindowFocusEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		std::string m_Name;
		
		bool m_Running = true;
		bool m_Paused = false; // Whether the game is running in a freezed state
		bool m_Minimized = false;
		bool m_Focused = true;

		bool m_PauseOnFocusLost;

		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		float m_FPS = 0.0f;

	private:
		static Application* s_Instance;
	};

	// To be defined in the client application
	Application* CreateApplication();

}

