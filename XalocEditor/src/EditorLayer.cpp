#include "EditorLayer.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>


namespace Xaloc {

	static const uint32_t s_mapWidth = 24;
	static const uint32_t s_mapHeight = 14;

	static const char* s_MapTiles =
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWW3TTTT2WWWWWWWWWWW"
		"WWWWW3T5GGGG4TTT2WWWWWWW"
		"WWWWW06GGGGG7BB642WWWWWW"
		"WWWWWWLGGGGGRWWLG42WWWWW"
		"WWWWWW06GGGGRWWLGGRWWWWW"
		"WWWWWWW0BB6G4TT5G71WWWWW"
		"WWWWWWWWWW0BB6GGGRWWWWWW"
		"WWWWWWWWWWWWW0BBB1WWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		"WWWWWWWWWWWWWWWWWWWWWWWW"
		;


	EditorLayer::EditorLayer()
		: Layer("Editor Layer"),
		//m_CameraController(1280.0f / 720.0f, true),
		m_TilingFactor(1.0f),
		m_Rotation(0.0f),
		m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
		m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
	{
		// Load Assets

		AssetManager::LoadTexture("TILEMAP", "assets/game/textures/tilemap.png");
		

		// Init Scene

		m_Scene = Scene::Load("assets/scenes/serializedScene.xaloc");

		m_FirstCamera = m_Scene->CreateEntity("Orthographic Camera");
		auto& orthoData = m_FirstCamera.AddComponent<OrthographicCameraDataComponent>();
		auto& firstCamera = m_FirstCamera.AddComponent<CameraComponent>(orthoData.CalculateProjectionMatrix());


		
		//m_SecondCamera = m_Scene->CreateEntity("Editor Camera");
		//auto& perspData = m_SecondCamera.AddComponent<PerspectiveCameraDataComponent>();
		//auto& secondCamera = m_SecondCamera.AddComponent<CameraComponent>(perspData.CalculateProjectionMatrix()); // (glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
		//secondCamera.Priority = 1;
		
		
		Xaloc::ScriptEngine::SetSceneContext(m_Scene);
		m_Scene->StartRuntime();

		

		
		// Init Editor

		m_EditorCameraData = {};
		m_EditorCameraData.Fov = 45.0f;
		m_EditorCameraData.Width = 1280.0f;
		m_EditorCameraData.Height = 720.0f;
		m_EditorCameraData.ZFar = 1000.0f;
		m_EditorCameraData.ZNear = 0.01f;

		m_EditorCameraTransform = {};
		m_EditorCameraTransform.Transform = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 10.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f });

		m_EditorCamera = CreateRef<Camera>(m_EditorCameraData.CalculateProjectionMatrix());
		
		
		//m_Scene = Xaloc::CreateRef<Scene>("Sandbox Scene");
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_Scene);

		m_GameViewport = CreateRef<EditorViewport>("Game Preview");
		m_SceneViewport = CreateRef<EditorViewport>("Scene");
		
		//m_CameraController.SetZoomLevel(5.0f);
	}


	void EditorLayer::OnAttach()
	{		
		m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");


		FramebufferSpec framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
		Ref<Framebuffer> framebuffer = Framebuffer::Create(framebufferSpec);

		RenderPassSpecification renderPassSpec;
		renderPassSpec.TargetFramebuffer = framebuffer;
		m_RenderPass = RenderPass::Create(renderPassSpec);

		
		Ref<Framebuffer> editorFramebuffer = Framebuffer::Create(framebufferSpec);
		RenderPassSpecification editorRenderPassSpec;
		editorRenderPassSpec.TargetFramebuffer = editorFramebuffer;
		
		m_EditorRenderPass = RenderPass::Create(editorRenderPassSpec);


		m_GuizmoRenderPass = RenderPass::Create(renderPassSpec);



		Xaloc::Ref<Xaloc::Texture2D> tilemap = AssetManager::GetTexture("TILEMAP");

		glm::vec2 size = { 16.0f, 16.0f };
		glm::vec2 pad = { 0.0f, 0.0f };
		glm::vec2 off = { 1.0f, 1.0f };

		s_TextureMap['G'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 16.0f }, size, pad, off);

		s_TextureMap['T'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 17.0f }, size, pad, off);
		s_TextureMap['B'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 15.0f }, size, pad, off);
		s_TextureMap['L'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 16.0f }, size, pad, off);
		s_TextureMap['R'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 16.0f }, size, pad, off);

		s_TextureMap['0'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 15.0f }, size, pad, off);
		s_TextureMap['1'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 15.0f }, size, pad, off);
		s_TextureMap['2'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 17.0f }, size, pad, off);
		s_TextureMap['3'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 17.0f }, size, pad, off);

		s_TextureMap['4'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 5.0f, 16.0f }, size, pad, off);
		s_TextureMap['5'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 6.0f, 16.0f }, size, pad, off);
		s_TextureMap['6'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 6.0f, 17.0f }, size, pad, off);
		s_TextureMap['7'] = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 5.0f, 17.0f }, size, pad, off);

		m_TileWater = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 9.0f, 10.0f }, size, pad, off);


	}

	void EditorLayer::OnDetach()
	{
	}


	void EditorLayer::OnUpdate(Xaloc::Timestep ts)
	{
		// UPDATE

		bool blockEv = !(m_GameViewport->IsFocused() && m_GameViewport->IsHovered());
		
		Application::Get().GetImGuiLayer()->SetBlockEvents(blockEv);
		if (m_GameViewport->IsFocused())
		{
			// TODO update editor camera
			//m_CameraController.OnUpdate(ts);
		}


		// RENDER

		Xaloc::Renderer2D::ResetStats();


		// UPDATE AND RENDER SCENE
		
		Renderer::BeginRenderPass(m_RenderPass);
		m_Scene->OnUpdate(ts);
		Renderer::EndRenderPass();


		
		// RENDER SCENE FROM EDITOR CAMERA

		Renderer::BeginRenderPass(m_EditorRenderPass);
		m_Scene->RenderScene(*m_EditorCamera.get(), m_EditorCameraTransform.Transform);
		Renderer::EndRenderPass();

		
		// TODO DRAW SELECTED ENTITIES GUI
		/*
		if (m_SelectionContext.size())
		{
			Camera* mainCamera = m_Scene->GetMainCamera();

			if (mainCamera)
			{
				Entity selection = m_SelectionContext[0].Entity;

				Xaloc::Renderer::BeginRenderPass(m_GuizmoRenderPass, false);
				Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());

				// TODO get app pixels per unit
				float pxPerUnit = 16.0f;
				float offset = 1.0f / pxPerUnit;

				glm::vec4 sprMin = selection.Transform() * glm::vec4{ -0.5f - offset, -0.5f - offset, 0.0f, 1.0f };        // Get sprite quad min vertex
				glm::vec4 sprMax = selection.Transform() * glm::vec4{ 0.5f + offset, 0.5f + offset, 0.0f, 1.0f };          // Get sprite quad max vertex

				glm::vec4 color = { 0.549f, 0.976f, 1.0f, 0.9f };
				//glm::vec4 color = { 1.0f, 1.0f, 1.0f, 0.9f };

				Renderer2D::DrawLine({ sprMin.x, sprMin.y }, { sprMax.x, sprMin.y }, color, 0.5f);
				Renderer2D::DrawLine({ sprMax.x, sprMin.y }, { sprMax.x, sprMax.y }, color, 0.5f);
				Renderer2D::DrawLine({ sprMax.x, sprMax.y }, { sprMin.x, sprMax.y }, color, 0.5f);
				Renderer2D::DrawLine({ sprMin.x, sprMax.y }, { sprMin.x, sprMin.y }, color, 0.5f);

				Xaloc::Renderer2D::EndScene();
				Xaloc::Renderer::EndRenderPass();
			}
		}
		*/
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
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
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
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}


		
		// ============================================= MENU BAR ============================================= //

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Xaloc"))
			{
				if (ImGui::MenuItem("Exit")) Xaloc::Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				//if (ImGui::MenuItem("Save"))
				//{
					// TODO
				//}
				if (ImGui::MenuItem("Save as..."))
				{
					std::string filename = Application::Get().SaveFile("*.xaloc");
					if (filename != "")
						Scene::Save(m_Scene, filename);
				}
				if (ImGui::MenuItem("Load..."))
				{
					std::string filename = Application::Get().OpenFile("*.xaloc");
					if (filename != "")
					{
						m_Scene.reset();
						m_Scene = Scene::Load(filename);
						m_SceneHierarchyPanel->SetScene(m_Scene);
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Console")) Application::Get().GetImGuiLayer()->ShowConsole();
				if (ImGui::MenuItem("Profiler")) Application::Get().GetImGuiLayer()->ShowProfiler();
				if (ImGui::MenuItem("Render Stats")) m_ShowWindowRenderStats = true;
				ImGui::EndMenu();
			}
			
			ImGui::EndMenuBar();
		}



		


		// ============================================= MISC WINDOWS ============================================= //

		ImGui::Begin("Settings");
		ImGui::DragFloat("Tiling factor", &m_TilingFactor, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Rotation", &m_Rotation, 0.1f);
		ImGui::ColorEdit4("First Color", glm::value_ptr(m_FirstColor));
		ImGui::ColorEdit4("Second Color", glm::value_ptr(m_SecondColor));

		ImGui::Separator();

		ImGui::DragFloat("Tiles depth", &m_TilesDepth, 0.1f, -10.0f, 10.0f);

		ImGui::End();


		if (m_ShowWindowRenderStats)
		{
			auto stats = Xaloc::Renderer2D::GetStats();
			
			ImGui::Begin("Render Stats", &m_ShowWindowRenderStats);
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::End();
		}




		// ============================================= HIERARCHY ============================================= //

		m_SceneHierarchyPanel->OnImGuiRender();
		

		
		// ============================================= VIEWPORTS ============================================= //

		if (m_GameViewport->Render(m_RenderPass))
		{
			// TODO OnWindow resize, call m_CameraController.OnResize again 
			//m_CameraController.OnResize(viewportSize.x, viewportSize.y);
		}

		if (m_SceneViewport->Render(m_EditorRenderPass))
		{
			m_EditorCameraData.Width = m_SceneViewport->GetSize().x;
			m_EditorCameraData.Height = m_SceneViewport->GetSize().y;
			m_EditorCamera = CreateRef<Camera>(m_EditorCameraData.CalculateProjectionMatrix());
		}

		/*
		ImGui::Begin("Viewport Info");
		ImGui::Text("Min bound: [%f, %f]", m_ViewportBounds[0].x, m_ViewportBounds[0].y);
		ImGui::Text("Max bound: [%f, %f]", m_ViewportBounds[1].x, m_ViewportBounds[1].y);
		ImGui::Separator();
		ImGui::Text("Window size: [%f, %f]", viewportSize.x, viewportSize.y);
		ImGui::End();
		*/



		
		

		// ============================================= GUIZMO ============================================= //

		if (m_SelectionContext.size())
		{
			// TODO draw guizmo

			/*
			auto& selection = m_SelectionContext[0];

			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

			// TODO use camera controller
			auto& camera = m_CameraController.GetCamera();
			bool snap = false; // TODO Input::IsKeyPressed(HZ_KEY_LEFT_CONTROL);

			auto& entityTransform = selection.Transform();
			
			// TODOfloat snapValue[3] = { m_SnapValue, m_SnapValue, m_SnapValue };
			float snapValue[3] = { 0.1, 0.1, 0.1 };
			
			//if (m_SelectionMode == SelectionMode::Entity)
			{
				ImGuizmo::Manipulate(glm::value_ptr(camera.GetViewMatrix()),
					glm::value_ptr(camera.GetProjectionMatrix()),
					(ImGuizmo::OPERATION)m_GizmoType,
					ImGuizmo::LOCAL,
					glm::value_ptr(entityTransform),
					nullptr,
					snap ? snapValue : nullptr);
			}
			*/
		}

	

		ImGui::End();

	}

	void EditorLayer::OnEvent(Xaloc::Event& e)
	{
		// TODO remove this
		//m_CameraController.OnEvent(e);

		m_Scene->OnEvent(e);
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(XA_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));

		dispatcher.Dispatch<MouseScrolledEvent>(XA_BIND_EVENT_FN(EditorLayer::OnMouseScroled));
		dispatcher.Dispatch<WindowResizeEvent>(XA_BIND_EVENT_FN(EditorLayer::OnWindowResized));
	}


	bool EditorLayer::OnMouseScroled(MouseScrolledEvent& e)
	{
		// TODO move editor camera back and forward
		return false;
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& e)
	{
		m_EditorCameraData.Width = (float)e.GetWidth();
		m_EditorCameraData.Height = (float)e.GetHeight();
		m_EditorCamera = CreateRef<Camera>(m_EditorCameraData.CalculateProjectionMatrix());
		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto [mx, my] = Input::GetMousePosition();
		if (e.GetMouseButton() == XA_MOUSE_BUTTON_LEFT) // TODO && !Input::IsKeyPressed(KeyCode::LeftAlt) && !ImGuizmo::IsOver())
		{
			auto [mouseX, mouseY] = GetMouseViewportSpace();
			if (mouseX > -1.0f && mouseX < 1.0f && mouseY > -1.0f && mouseY < 1.0f) // If mouse is on viewport range
			{				
				m_SelectionContext.clear();
				auto spriteEntities = m_Scene->FindEntitiesWith<SpriteRendererComponent>();
				for (auto e : spriteEntities)
				{
					Entity entity = { e, m_Scene.get() };
					auto tex = entity.GetComponent<SpriteRendererComponent>().SubTexture;
					if (!tex)
						continue;
					
					// TODO use pixels per unit and SubTexture width and size
					AABB boundingBox;
					glm::vec4 sprMin = entity.Transform() * glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f };        // Get sprite quad min vertex
					glm::vec4 sprMax = entity.Transform() * glm::vec4{ 0.5f, 0.5f, 0.0f, 1.0f };          // Get sprite quad max vertex

					//                   Projection matrix               * View matrix
					glm::mat4 viewProj = m_EditorCamera->GetProjection() * glm::inverse(m_EditorCameraTransform.Transform);

					boundingBox.Min = sprMin * viewProj;  // Translate it to screen space
					boundingBox.Max = sprMax * viewProj;  // Translate it to screen space

					//boundingBox.Min = sprMin * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space
					//boundingBox.Max = sprMax * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space
										
					bool intersects = mouseX > boundingBox.Min.x && mouseX < boundingBox.Max.x
									&& mouseY > boundingBox.Min.y && mouseY < boundingBox.Max.y;

					// TODO don't intersect if alpha == 0
					
					if (intersects)
					{
						m_SelectionContext.push_back({ entity });
					}
				}
				// TODO std::sort(m_SelectionContext.begin(), m_SelectionContext.end(), [](auto& a, auto& b) { return a.Distance < b.Distance; });
				if (m_SelectionContext.size())
					OnEntitySelected(m_SelectionContext[0]);
			}
		}
		
		return false;
	}


	void EditorLayer::OnEntitySelected(const SelectedEntity& selection)
	{
		XA_CORE_TRACE("Entity Selected: {0}", (uint32_t)selection.Entity);
		m_SceneHierarchyPanel->SetSelected(selection.Entity);
	}

	

	// TODO move to EditorViewport
	std::pair<float, float> EditorLayer::GetMouseViewportSpace()
	{
		glm::vec2 bounds0 = m_SceneViewport->GetBounds(0);
		glm::vec2 bounds1 = m_SceneViewport->GetBounds(1);
		
		if (!m_SceneViewport->IsHovered() && m_GameViewport->IsHovered())
		{
			bounds0 = m_GameViewport->GetBounds(0);
			bounds1 = m_GameViewport->GetBounds(1);
		}

		
		auto [mx, my] = ImGui::GetMousePos(); // Input::GetMousePosition();
		mx -= bounds0.x;
		my -= bounds0.y;
		auto viewportWidth = bounds1.x - bounds0.x;
		auto viewportHeight = bounds1.y - bounds0.y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}


}
