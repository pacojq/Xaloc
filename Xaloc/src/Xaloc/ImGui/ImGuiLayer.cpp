#include "xapch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "imgui_internal.h"

#include "Xaloc/Core/Application.h"
#include "Xaloc/Core/Log.h"

#include "Platform/OpenGL/OpenGLImGuiLayer.h"


namespace Xaloc {


	ImGuiLayer* ImGuiLayer::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;

		case RendererAPI::API::OpenGL:
			return new OpenGLImGuiLayer();

		}
		XA_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
		m_Profiler = CreateRef<ImGuiProfiler>();
	}



	void ImGuiLayer::SetupImGui()
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
	}


	
	
	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

}
