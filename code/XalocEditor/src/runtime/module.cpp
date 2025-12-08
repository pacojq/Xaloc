#include "runtime/module.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/pipeline/renderpass.h"
#include "xaloc/scripting/engine.h"
#include "tooling/imgui/imguizmo.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Xaloc;


namespace Editor
{

ZModuleEditorRuntime::ZModuleEditorRuntime(const ZRef<ZScene>& scene) : ZModule("Editor Runtime")
{
	// Init Scene
	// TODO save current scene state
	m_Scene = scene;

	Scripting::ZScriptEngine::SetSceneContext(m_Scene);
	m_Scene->StartRuntime();
}

void ZModuleEditorRuntime::OnAttach()
{
	SFramebufferDefinition framebuffer;
	framebuffer.nWidth = 1280;
	framebuffer.nHeight = 720;
	framebuffer.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
	m_pFramebuffer = ZFramebuffer::Create(framebuffer);

	SRenderPassDefinition renderPass;
	renderPass.pTarget = m_pFramebuffer;
	m_pRenderPass = ZRenderPass::Create(renderPass);
}

void ZModuleEditorRuntime::OnDetach()
{
	// TODO reload state before playing
	delete m_pFramebuffer;
	delete m_pRenderPass;
}

void ZModuleEditorRuntime::OnUpdate(ZTimestep ts)
{
	RendererBeginPass(m_pRenderPass);
	m_Scene->OnUpdateRuntime(ts);
	RendererEndPass();
}

void ZModuleEditorRuntime::OnImGuiRender()
{
	// VIEWPORTS
	/*
	m_GameViewport->Begin();
	if (m_GameViewport->Render(m_RenderPass))
	{
		// TODO OnWindow resize, call m_CameraController.OnResize again 
		//m_CameraController.OnResize(viewportSize.x, viewportSize.y);

		auto& data = m_MainCamera.GetComponent<CameraComponent>();
		uint32_t width = (uint32_t)m_GameViewport->GetSize().x;
		uint32_t height = (uint32_t)m_GameViewport->GetSize().y;
		data.Camera.SetViewportSize(width, height);
	}
	m_GameViewport->End();
	*/
}

void ZModuleEditorRuntime::OnEvent(ZEvent& rEvent)
{
	m_Scene->OnEvent(rEvent);

	ZEventDispatcher dispatcher(rEvent);
	dispatcher.Dispatch<EvtWindowResize>(XA_BIND_EVENT_FN(ZModuleEditorRuntime::OnWindowResized));
}

bool ZModuleEditorRuntime::OnWindowResized(EvtWindowResize& rEvent)
{
	if (rEvent.IsMinimized()) // Minimized
	{
		return false;
	}
	// TODO when dragging the scene viewport to dock it somewhere else, the camera rotates in a weird way

	return false;
}

}
