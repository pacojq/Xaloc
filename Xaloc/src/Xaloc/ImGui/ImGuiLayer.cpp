#include "xapch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Xaloc/Core/Application.h"
#include "Xaloc/Core/Log.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>


namespace Xaloc {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}



	void ImGuiLayer::OnAttach()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	// Enable keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	// Enable Gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		// Enable Viewports
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tewak WindowRounding/WindowBg so platform windows can look identical to regular ones
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		//ImGuiIO& io = ImGui::GetIO();
		//e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
		//e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
	
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}






	void ImGuiLayer::OnImGuiRender()
	{
		//static bool show = false;
		//ImGui::ShowDemoWindow(&show);

		static bool showConsole = true;
		ImGuiConsole::OnImGuiRender(&showConsole);

		RenderFPS();
	}


	
	void ImGuiLayer::RenderFPS()
	{
		float avg = 0;
		float min = 300;
		float max = 0;

		uint32_t size = m_FrameTimes.size();
		if (size >= 50)
			m_FrameTimes.erase(m_FrameTimes.begin());

		//m_FrameTimes.push_back(Xaloc::Application::GetFPS());
		m_FrameTimes.push_back(ImGui::GetIO().Framerate);
		for (int i = 0; i < size; i++)
		{
			m_FpsValues[i] = m_FrameTimes[i];
			avg += m_FrameTimes[i];
		}
		avg /= size;
		


		ImGui::Begin("FPS");
		ImGui::PlotLines("FPS", m_FpsValues, size);
		ImGui::Text("FPS: %f", avg);
		ImGui::Text("Frame time (ms): %f", 1.0f / avg * 1000.0f);

		bool vSync = Application::Get().GetWindow().IsVSync();
		ImGui::Checkbox("VSync Enabled", &vSync);
		Application::Get().GetWindow().SetVSync(vSync);
		
		ImGui::End();
	}


}