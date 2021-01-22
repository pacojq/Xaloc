#include "xapch.h"
#ifdef XA_IMGUI_DIRECTX
#include "DirectXImGuiLayer.h"

#include "Xaloc/Core/Application.h"

#include "Xaloc/Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"

namespace Xaloc {

	DirectXImGuiLayer::DirectXImGuiLayer()
		: ImGuiLayer()
	{
	}


	


	void DirectXImGuiLayer::OnAttach()
	{
		SetupImGui();

		Application& app = Application::Get();

		// Setup Platform/Renderer bindings
		// TODO ImGui_ImplWin32_Init(app.GetWindow().GetNativeWindow());
		// TODO ImGui_ImplDX11_Init(dxContext.GetDevice(), dxContext.GetDeviceContext());
	}

	void DirectXImGuiLayer::OnDetach()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}



	void DirectXImGuiLayer::Begin()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void DirectXImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		// TODO auto rtv = dxContext.GetMainRenderTargetView();
		// TODO dxContext.GetDeviceContext()->OMSetRenderTargets(1, &rtv, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}





	void DirectXImGuiLayer::OnImGuiRender()
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