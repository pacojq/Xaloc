#include "xapch.h"
#ifdef XA_IMGUI_GLFW
#include "OpenGLImGuiLayer.h"

#include "Xaloc/Core/Application.h"

#include "Xaloc/Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

namespace Xaloc {

	OpenGLImGuiLayer::OpenGLImGuiLayer()
		: ImGuiLayer()
	{
	}


	


	void OpenGLImGuiLayer::OnAttach()
	{
		SetupImGui();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 450");
	}

	void OpenGLImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



	void OpenGLImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void OpenGLImGuiLayer::End()
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





	void OpenGLImGuiLayer::OnImGuiRender()
	{
		//static bool show = false;
		//ImGui::ShowDemoWindow(&show);

		if (m_ShowConsole)
		{
			ImGuiConsole::OnImGuiRender(&m_ShowConsole);
		}
		if (m_ShowProfiler)
		{
			m_Profiler->OnImGuiRender(&m_ShowProfiler);
		}


		ImGui::Begin("Graphics");

		bool vSync = Application::Get().GetWindow().IsVSync();
		ImGui::Checkbox("VSync Enabled", &vSync);
		Application::Get().GetWindow().SetVSync(vSync);

		ImGui::End();
	}

	
}
#endif