#include "EditorLayer.h"

#include "EditorNames.h"

#include "Xaloc/Math/Math.h"
#include "Xaloc/Files/FileSystem.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#include <glm/gtc/type_ptr.hpp>


namespace Xaloc {


	EditorLayer::EditorLayer() : Layer("Editor Layer")
	{
		m_MenuBar = CreateRef<EditorMenuBar>(this);
		
		// TODO: start with empty room
		auto& scene = Scene::Load("assets/game/scenes/serializedScene.xaloc");
		OpenScene(scene);

		// Init Editor
		m_EditorCamera = CreateRef<EditorCamera>();

		m_Icons = CreateRef<EditorIcons>();

		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(this, scene);
		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>(this);

		m_GamePreview = CreateRef<EditorGamePreview>(EditorNames::Windows::GAME_PREVIEW);
		m_SceneViewport = CreateRef<EditorSceneView>(EditorNames::Windows::SCENE);
		m_SceneViewport->SetScene(scene);
		

		m_PanelsInitialized = true;
	}


	void EditorLayer::OnAttach()
	{
		FramebufferSpec framebufferSpec; // TODO: available width and height
		framebufferSpec.Width = 480 * 2;
		framebufferSpec.Height = 270 * 2;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
		Ref<Framebuffer> previewframebuffer = Framebuffer::Create(framebufferSpec);
		Ref<Framebuffer> editorFramebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification previewRenderPassSpec;
		previewRenderPassSpec.TargetFramebuffer = previewframebuffer;

		RenderPassSpecification editorRenderPassSpec;
		editorRenderPassSpec.TargetFramebuffer = editorFramebuffer;

		m_PreviewRenderPass = RenderPass::Create(previewRenderPassSpec);
		m_EditorRenderPass = RenderPass::Create(editorRenderPassSpec);


		const glm::vec3 m_BgColA = { 78.0f / 255.0f, 78.0f / 255.0f, 78.0f / 255.0f };
		const glm::vec3 m_BgColB = { 83.0f / 255.0f, 83.0f / 255.0f, 83.0f / 255.0f };

		m_CheckerboardShader = Shader::Create("assets/shaders/CheckerboardBlit.glsl");
		m_CheckerboardShader->Bind();
		m_CheckerboardShader->SetFloat("u_PxPerUnit", Renderer2D::PX_PER_UNIT);
		m_CheckerboardShader->SetFloat3("u_ColorLight", m_BgColA);
		m_CheckerboardShader->SetFloat3("u_ColorDark", m_BgColB);
		m_CheckerboardShader->SetFloat4("u_ScreenSize", { framebufferSpec.Width, framebufferSpec.Height, 0.0f, 0.0f });
	}

	void EditorLayer::OnDetach()
	{
	}





	void EditorLayer::OpenScene(const Ref<Scene>& scene)
	{
		m_Scene.reset();
		m_Scene = scene;

		if (m_PanelsInitialized)
		{
			m_SceneHierarchyPanel->SetScene(scene);
			m_SceneViewport->SetScene(scene);
		}

#ifdef TODO // TODO: temporary
		{
			Entity player = m_Scene->FindEntityByName("Player");
			if (player)
			{
				XA_CORE_ERROR("ADDING PLAYER COMPONENT!");
				player.AddComponent<NativeBehaviourComponent>().Bind<PlayerComponent>();
			}
		}
#endif
		// TODO: temporary
		{
			SceneCamera& cam = m_Scene->GetCameraStack()->MainCamera();
			cam.Camera.SetViewportSize(480, 270);
			cam.Camera.SetClipDistance(-100, 100);
		}

		// vv ======================================== TODO temporary: start runtime
		ScriptEngine::SetSceneContext(m_Scene);
		m_Scene->StartRuntime();
		// ^^ ============================================= temporary: start runtime
	}





	void EditorLayer::OnUpdate(Timestep ts)
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
		
		Application::Get().GetImGuiLayer()->SetBlockEvents(blockEvents);
		if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
		{
			m_EditorCamera->OnUpdate(ts);
		}
		

		// RENDER

		Xaloc::Renderer2D::ResetStats();


		// UPDATE AND RENDER SCENE

		SceneCamera& mainCamera = m_Scene->GetCameraStack()->MainCamera();
		if (mainCamera.IsActive())
		{
			Renderer::BeginRenderPass(m_PreviewRenderPass);
			{
				m_Scene->OnUpdateEditor(ts, mainCamera.ViewProjectionMatrix());
			}
			Renderer::EndRenderPass();
		}


		
		// RENDER SCENE FROM EDITOR CAMERA

		Renderer::BeginRenderPass(m_EditorRenderPass, true);
		{
			Renderer2D::Blit(m_CheckerboardShader); // Editor background
		}
		Renderer::EndRenderPass();

		Renderer::BeginRenderPass(m_EditorRenderPass, false);
		{
			m_Scene->OnUpdateEditor(ts, m_EditorCamera->GetViewProjection());
		}
		Renderer::EndRenderPass();
	}



	void EditorLayer::OnImGuiRender()
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
			ImGui::PopStyleVar(2);

		// DockSpace
		m_DockspaceId = ImGui::GetID("MyDockSpace");
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGui::DockSpace(m_DockspaceId, ImVec2(0.0f, 0.0f), dockspace_flags);

			if (!m_DockspaceReady)
				ResetDockSpace();
		}


		m_MenuBar->OnImGuiRender();

			

		// ============================================= MISC WINDOWS ============================================= //

		if (m_ShowWindowRenderStats)
		{
			auto stats = Xaloc::Renderer2D::GetStats();
			
			ImGui::Begin("Render Stats", &m_ShowWindowRenderStats);
			{
				ImGui::Text("Draw Calls: %d", stats.DrawCalls);
				ImGui::Text("Quads: %d", stats.QuadCount);
				ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
				ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				ImGui::End();
			}
		}

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


		// ============================================= HIERARCHY ============================================= //

		m_SceneHierarchyPanel->OnImGuiRender();

		m_ContentBrowserPanel->OnImGuiRender();
		
		
		// ============================================= VIEWPORTS ============================================= //

		m_GamePreview->OnImGuiDraw(m_PreviewRenderPass->GetSpecification().TargetFramebuffer);


		Ref<Framebuffer>& roomViewFramebuffer = m_EditorRenderPass->GetSpecification().TargetFramebuffer;
		Entity selectedEntity = m_SceneHierarchyPanel->GetSelectedEntity();

		if (m_SceneViewport->OnImGuiDraw(roomViewFramebuffer, m_EditorCamera, selectedEntity))
		{
			uint32_t width = (uint32_t)m_SceneViewport->GetSize().x;
			uint32_t height = (uint32_t)m_SceneViewport->GetSize().y;
			m_EditorCamera->SetViewportSize(width, height);

			m_CheckerboardShader->Bind();
			m_CheckerboardShader->SetFloat4("u_ScreenSize", { width, height, 0.0f, 0.0f });
		}

		
		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Scene->OnEvent(e);

		if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
		{
			m_EditorCamera->OnEvent(e);
		}
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(XA_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<WindowResizeEvent>(XA_BIND_EVENT_FN(EditorLayer::OnWindowResized));
	}



	

	bool EditorLayer::OnWindowResized(WindowResizeEvent& e)
	{
		if (e.GetWidth() <= 0.0f || e.GetHeight() <= 0.0f) // Minimized
			return false;

		// TODO when dragging the scene viewport to dock it somewhere else, the camera rotates in a weird way

		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();
		m_EditorCamera->SetViewportSize(width, height);
		
		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto [mx, my] = Input::GetMousePosition();
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
				auto spriteEntities = m_Scene->FindEntitiesWith<SpriteRendererComponent>();
				for (auto e : spriteEntities)
				{
					Entity entity = { e, m_Scene.get() };
					auto tex = entity.GetComponent<SpriteRendererComponent>().SubTexture;
					if (!tex)
						continue;
					
					// TODO use pixels per unit and SubTexture width and size
					TransformComponent trans = entity.GetComponent<TransformComponent>();
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




	void EditorLayer::ResetDockSpace()
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


		ImGui::DockBuilderDockWindow(EditorNames::Windows::GAME_PREVIEW, dock_main_id);
		ImGui::DockBuilderDockWindow(EditorNames::Windows::SCENE, dock_main_id);

		ImGui::DockBuilderDockWindow(EditorNames::Windows::PROJECT, dock_right);

		ImGui::DockBuilderDockWindow(EditorNames::Windows::SCENE_HIERARCHY, dock_left_top);
		ImGui::DockBuilderDockWindow(EditorNames::Windows::PROPERTIES, dock_left_bot);

		ImGui::DockBuilderDockWindow(EditorNames::Windows::CONSOLE, dock_bottom);
		ImGui::DockBuilderDockWindow("Profiler", dock_bottom);
		ImGui::DockBuilderDockWindow("Graphics", dock_bottom);


		ImGui::DockBuilderFinish(m_DockspaceId);
	}


}
