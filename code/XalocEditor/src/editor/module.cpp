#include "editor/module.h"
#include "editor/names.h"
#include "tooling/imgui/imguizmo.h"
#include "xaloc/core/application.h"
#include "xaloc/core/input/input.h"
#include "xaloc/math/math.h"
#include "xaloc/scripting/engine.h"
#include "xaloc/renderer/renderer2D.h"
#include "xaloc/renderer/debugging/renderer2D.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>

using namespace Xaloc;


namespace Editor
{

ZModuleEditor::ZModuleEditor() : ZModule("Editor")
{
	m_MenuBar = CreateRef<ZEditorMenuBar>(this);

	// TODO: start with empty room
	auto& scene = ZScene::Load("assets/game/scenes/serializedScene.xaloc");
	OpenScene(scene);

	// Init Editor
	m_EditorCamera = CreateRef<ZEditorCamera>();

	m_Icons = CreateRef<ZEditorIcons>();

	m_SceneHierarchyPanel = CreateScope<ZPanelSceneHierarchy>(this, scene);
	m_ContentBrowserPanel = CreateScope<ZPanelContentBrowser>(this);

	m_GamePreview = CreateRef<ZEditorGamePreview>(Names::Windows::GAME_PREVIEW);
	m_SceneViewport = CreateRef<ZEditorSceneView>(Names::Windows::SCENE);
	m_SceneViewport->SetScene(scene);

	m_PanelsInitialized = true;
}

void ZModuleEditor::OnAttach()
{
	SFramebufferDefinition framebuffer; // TODO: available width and height
	framebuffer.nWidth = 480 * 2;
	framebuffer.nHeight = 270 * 2;
	framebuffer.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };

	m_pFramebufferPreview = ZFramebuffer::Create(framebuffer);
	m_pFramebufferEditor = ZFramebuffer::Create(framebuffer);

	SRenderPassDefinition renderPassPreview;
	renderPassPreview.pTarget = m_pFramebufferPreview;

	SRenderPassDefinition renderPassEditor;
	renderPassEditor.pTarget = m_pFramebufferEditor;

	m_pRenderPassPreview = ZRenderPass::Create(renderPassPreview);
	m_pRenderPassEditor = ZRenderPass::Create(renderPassEditor);

	const glm::vec3 m_BgColA = { 78.0f / 255.0f, 78.0f / 255.0f, 78.0f / 255.0f };
	const glm::vec3 m_BgColB = { 83.0f / 255.0f, 83.0f / 255.0f, 83.0f / 255.0f };

	m_CheckerboardShader = ZShader::Create("assets/shaders/CheckerboardBlit.glsl");
	m_CheckerboardShader->Bind();
	m_CheckerboardShader->SetFloat("u_PxPerUnit", C_PIXELS_PER_UNIT);
	m_CheckerboardShader->SetFloat3("u_ColorLight", m_BgColA);
	m_CheckerboardShader->SetFloat3("u_ColorDark", m_BgColB);
	m_CheckerboardShader->SetFloat4("u_ScreenSize", { framebuffer.nWidth, framebuffer.nHeight, 0.0f, 0.0f });
}

void ZModuleEditor::OnDetach()
{
	delete m_pFramebufferEditor;
	delete m_pRenderPassEditor;

	delete m_pFramebufferPreview;
	delete m_pRenderPassPreview;
}

void ZModuleEditor::OpenScene(const ZRef<ZScene>& scene)
{
	m_Scene.reset();
	m_Scene = scene;

	if (m_PanelsInitialized)
	{
		m_SceneHierarchyPanel->SetScene(scene);
		m_SceneViewport->SetScene(scene);
	}

	// TODO: temporary
	{
		SSceneCamera& rMainCamera = m_Scene->GetCameraStack()->MainCamera();
		rMainCamera.Camera.SetPixelSize(480, 270);
		rMainCamera.Camera.SetClipDistance(-100, 100);
	}

	// vv ======================================== TODO temporary: start runtime
	Scripting::ZScriptEngine::SetSceneContext(m_Scene);
	m_Scene->StartRuntime();
	// ^^ ============================================= temporary: start runtime
}

void ZModuleEditor::OnUpdate(ZTimestep ts)
{
	// UPDATE

	// TODO block events just to the viewport that doesn't need them

	bool blockEvents = true;

	if (m_GamePreview->IsFocused() && m_GamePreview->IsHovered())
	{
		blockEvents = false;
	}
	if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
	{
		blockEvents = false;
	}

	ZApplication::Get()->GetImGui()->SetBlockEvents(blockEvents);
	if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
	{
		m_EditorCamera->OnUpdate(ts);
	}

	// RENDER

#if RENDER_STATS_ENABLED
	ZDebugRenderer2D* pRenderDebug = ZRenderer2D::GetDebug();
	if (pRenderDebug)
	{
		pRenderDebug->ResetStats();
	}
#endif

	// UPDATE AND RENDER SCENE

	SSceneCamera& rMainCamera = m_Scene->GetCameraStack()->MainCamera();
	if (rMainCamera.IsActive())
	{
		RendererBeginPass(m_pRenderPassPreview);
		{
			m_Scene->OnUpdateEditor(ts, rMainCamera.ViewProjectionMatrix());
		}
		RendererEndPass();
	}

	// RENDER SCENE FROM EDITOR CAMERA

	RendererBeginPass(m_pRenderPassEditor, true);
	{
		ZRenderer2D::Blit(m_CheckerboardShader); // Editor background
	}
	RendererEndPass();

	RendererBeginPass(m_pRenderPassEditor, false);
	{
		m_Scene->OnUpdateEditor(ts, m_EditorCamera->GetViewProjection());
	}
	RendererEndPass();
}

void ZModuleEditor::OnImGuiRender()
{
	static bool p_open = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
	{
		ImGui::PopStyleVar(2);
	}

	// DockSpace
	m_DockspaceId = ImGui::GetID("MyDockSpace");
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(m_DockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (!m_DockspaceReady)
		{
			ResetDockSpace();
		}
	}

	m_MenuBar->OnImGuiRender();

	// MISC WINDOWS

#if RENDER_STATS_ENABLED
	const ZDebugRenderer2D* pRenderDebug = ZRenderer2D::GetDebug();
	if (m_ShowWindowRenderStats && pRenderDebug)
	{
		const SStatsRenderer2D& stats = pRenderDebug->ReadStats();

		ImGui::Begin("Render Stats", &m_ShowWindowRenderStats);
		{
			ImGui::Text("Draw Calls: %d", stats.nDrawCalls);
			ImGui::Text("Quads: %d", stats.nQuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::End();
		}
	}
#endif

	// TODO Play / Pause

#if TODO_RUNTIME
	ImGui::Begin("Runtime");
		
	if (m_OnRuntime)
	{
		if (ImGui::Button("Stop"))
		{
			Xaloc::Application::Get().PopLayer(m_RuntimeLayer);
			//delete m_RuntimeLayer;
			m_OnRuntime = false;
		}
	}
	else
	{
		if (ImGui::Button("Play"))
		{
			if (m_RuntimeLayer != nullptr)
			{
				delete m_RuntimeLayer;
				m_RuntimeLayer = nullptr;
			}

			m_RuntimeLayer = new RuntimeLayer(m_Scene);
			Xaloc::Application::Get().PushLayer(m_RuntimeLayer);
			m_OnRuntime = true;
		}
	}
	ImGui::End();
#endif

	// HIERARCHY
	m_SceneHierarchyPanel->OnImGuiRender();
	m_ContentBrowserPanel->OnImGuiRender();

	// VIEWPORTS
	m_GamePreview->OnImGuiDraw(m_pFramebufferPreview);

	ZEntity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();
	const bool bResize = m_SceneViewport->OnImGuiDraw(
		m_pFramebufferEditor,
		m_EditorCamera,
		selectedEntity);

	if (bResize)
	{
		uint32 nWidth = uint32(m_SceneViewport->GetSize().x);
		uint32 nHeight = uint32(m_SceneViewport->GetSize().y);
		m_EditorCamera->SetViewportSize(nWidth, nHeight);

		m_pFramebufferEditor->Resize(nWidth, nHeight);

		m_CheckerboardShader->Bind();
		m_CheckerboardShader->SetFloat4(
			"u_ScreenSize",
			{ nWidth, nHeight, 0.0f, 0.0f });
	}

	ImGui::End();
}

void ZModuleEditor::OnEvent(ZEvent& rEvent)
{
	m_Scene->OnEvent(rEvent);

	if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
	{
		m_EditorCamera->OnEvent(rEvent);
	}

	ZEventDispatcher dispatcher(rEvent);
	dispatcher.Dispatch<EvtMouseButtonPressed>(XA_BIND_EVENT_FN(ZModuleEditor::OnMouseButtonPressed));
	dispatcher.Dispatch<EvtWindowResize>(XA_BIND_EVENT_FN(ZModuleEditor::OnWindowResized));
}

bool ZModuleEditor::OnWindowResized(EvtWindowResize& e)
{
	if (e.GetWidth() <= 0.0f || e.GetHeight() <= 0.0f) // Minimized
	{
		return false;
	}

	// TODO when dragging the scene viewport to dock it somewhere else, the camera rotates in a weird way

	uint32 nWidth = e.GetWidth();
	uint32 nHeight = e.GetHeight();
	m_EditorCamera->SetViewportSize(nWidth, nHeight);

	return false;
}

bool ZModuleEditor::OnMouseButtonPressed(EvtMouseButtonPressed& e)
{
	auto [mx, my] = ZInput::GetMousePosition();
	if (e.GetMouseButton() == XA_MOUSE_BUTTON_LEFT) // TODO && !Input::IsKeyPressed(KeyCode::LeftAlt) && !ImGuizmo::IsOver())
	{
		auto [mouseX, mouseY] = m_SceneViewport->GetMouseViewportSpace();
		if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
		{
			auto [mx, my] = m_SceneViewport->GetMouseViewportSpace();
			mouseX = mx;
			mouseY = my;
		}

		// Editor Camera
		const glm::mat4& cameraProjection = m_EditorCamera->GetProjection();
		glm::mat4 cameraView = m_EditorCamera->GetViewMatrix();
			
		// TODO select entity
		/*
		if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f) // If mouse is on viewport range
		{
			auto spriteEntities = m_Scene->FindEntitiesWith<CmpSpriteRenderer>();
			for (auto e : spriteEntities)
			{
				ZEntity entity = { e, m_Scene.get() };
				auto tex = entity.GetComponent<CmpSpriteRenderer>().SubTexture;
				if (!tex)
					continue;
					
				// TODO use pixels per unit and SubTexture width and size
				CmpTransform trans = entity.GetComponent<CmpTransform>();
				AABB boundingBox;
				glm::vec4 sprMin = trans.GetTransform() * glm::vec4{ -0.5f * tex->GetWidth(), -0.5f * tex->GetHeight(), 0.0f, 1.0f };        // Get sprite quad min vertex
				glm::vec4 sprMax = trans.GetTransform() * glm::vec4{ 0.5f * tex->GetWidth(), 0.5f * tex->GetHeight(), 0.0f, 1.0f };          // Get sprite quad max vertex

				//                   Projection matrix               * View matrix
				glm::mat4 viewProj = m_EditorCamera.GetViewProjection();//m_EditorCamera->GetCamera()->GetProjection() * glm::inverse(m_EditorCamera->GetTransform().GetTransform());

				boundingBox.Min = sprMin * viewProj;  // Translate it to screen space
				boundingBox.Max = sprMax * viewProj;  // Translate it to screen space

				//boundingBox.Min = sprMin * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space
				//boundingBox.Max = sprMax * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space
										
				bool intersects = mouseX > boundingBox.Min.x && mouseX < boundingBox.Max.x
								&& mouseY > boundingBox.Min.y && mouseY < boundingBox.Max.y;

				// TODO don't intersect if alpha == 0
					
				if (intersects)
				{
					// TODO select using the scene hierarchy panel
					m_SceneHierarchyPanel->SetSelected(entity);
				}
			}
			// TODO std::sort(m_SelectionContext.begin(), m_SelectionContext.end(), [](auto& a, auto& b) { return a.Distance < b.Distance; });
		}
		*/
	}
	return false;
}

void ZModuleEditor::ResetDockSpace()
{
	m_DockspaceReady = true;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::DockBuilderRemoveNode(m_DockspaceId); // Clear out existing layout
	ImGui::DockBuilderAddNode(m_DockspaceId, ImGuiDockNodeFlags_DockSpace); // Add empty node
	ImGui::DockBuilderSetNodeSize(m_DockspaceId, viewport->Size);

	ImGuiID dock_main_id = m_DockspaceId;

	ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
	ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);
	ImGuiID dock_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
	//ImGuiID dock_top = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.20f, NULL, &dock_main_id);

	ImGuiID dock_left_top;
	ImGuiID dock_left_bot = ImGui::DockBuilderSplitNode(dock_left, ImGuiDir_Down, 0.50f, NULL, &dock_left_top);

	ImGui::DockBuilderDockWindow(Names::Windows::GAME_PREVIEW, dock_main_id);
	ImGui::DockBuilderDockWindow(Names::Windows::SCENE, dock_main_id);

	ImGui::DockBuilderDockWindow(Names::Windows::PROJECT, dock_right);

	ImGui::DockBuilderDockWindow(Names::Windows::SCENE_HIERARCHY, dock_left_top);
	ImGui::DockBuilderDockWindow(Names::Windows::PROPERTIES, dock_left_bot);

	ImGui::DockBuilderDockWindow(Names::Windows::CONSOLE, dock_bottom);
	ImGui::DockBuilderDockWindow("Profiler", dock_bottom);
	ImGui::DockBuilderDockWindow("Graphics", dock_bottom);

	ImGui::DockBuilderFinish(m_DockspaceId);
}

}
