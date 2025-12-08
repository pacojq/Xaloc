#include "xapch.h"
#include "tooling/imgui/module.h"
#include "tooling/imgui/core.h"
#include "platform/opengl/imguimodule.h"
#include "xaloc/core/application.h"
#include "xaloc/core/log.h"
#include "xaloc/debugging/assert.h"
#include "imgui_internal.h"


namespace Xaloc
{

ZModuleImGui* ZModuleImGui::Create()
{
	switch (RendererGetAPI())
	{
	case ERenderAPI::None:
		XA_CORE_ASSERT(false, "RendererAPI::None is not supported!");
		return nullptr;

	case ERenderAPI::OpenGL:
		return new ZModuleImGuiOpenGL();
	}
	XA_CORE_ASSERT(false, "Unknown RendererAPI");
	return nullptr;
}

ZModuleImGui::ZModuleImGui()
	: ZModule("ImGuiLayer")
{
	m_pProfiler = CreateRef<ZImGuiProfiler>();
}

void ZModuleImGui::SetupImGui()
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

void ZModuleImGui::OnEvent(ZEvent& rEvent)
{
	if (m_bBlockEvents)
	{
		ImGuiIO& io = ImGui::GetIO();
		rEvent.bHandled |= io.WantCaptureMouse
			&& rEvent.IsInCategory(EEventCategoryFlags::Mouse);
		rEvent.bHandled |= io.WantCaptureKeyboard
			&& rEvent.IsInCategory(EEventCategoryFlags::Keyboard);
	}
}

}
