#include "xapch.h"
#include "platform/opengl/imguimodule.h"
#include "xaloc/core/application.h"
#include "xaloc/renderer/renderer.h"
#include "tooling/imgui/core.h"
#include "tooling/imgui/imguizmo.h"

// Build OpenGL ImGui cpp files
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"

#include <GLFW/glfw3.h>


namespace Xaloc
{

ZModuleImGuiOpenGL::ZModuleImGuiOpenGL()
	: ZModuleImGui()
{
}

void ZModuleImGuiOpenGL::OnAttach()
{
	SetupImGui();

	ZApplication* pApp = ZApplication::Get();
	GLFWwindow* pWindow = static_cast<GLFWwindow*>(pApp->GetWindow().GetNativeWindow());

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

void ZModuleImGuiOpenGL::OnDetach()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ZModuleImGuiOpenGL::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ZModuleImGuiOpenGL::End()
{
	ImGuiIO& io = ImGui::GetIO();
	ZApplication* pApp = ZApplication::Get();
	io.DisplaySize = ImVec2((float)pApp->GetWindow().GetWidth(), (float)pApp->GetWindow().GetHeight());

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

void ZModuleImGuiOpenGL::OnImGuiRender()
{
	//static bool show = false;
	//ImGui::ShowDemoWindow(&show);

	if (m_bShowConsole)
	{
		ZImGuiConsole::OnImGuiRender(&m_bShowConsole);
	}
	if (m_bShowProfiler)
	{
		m_pProfiler->OnImGuiRender(&m_bShowProfiler);
	}

	ImGui::Begin("Graphics");

	bool bVSync = ZApplication::Get()->GetWindow().IsVSync();
	ImGui::Checkbox("VSync Enabled", &bVSync);
	ZApplication::Get()->GetWindow().SetVSync(bVSync);

	ImGui::End();
}

}