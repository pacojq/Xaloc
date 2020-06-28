#include "EditorLayer.h"

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
		m_CameraController(1280.0f / 720.0f, true),
		m_TilingFactor(1.0f),
		m_Rotation(0.0f),
		m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
		m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
	{
		m_Scene = Scene::Load("assets/scenes/serializedScene.xaloc");

		//m_Scene = Xaloc::CreateRef<Scene>("Sandbox Scene");
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_Scene);
		
		m_CameraController.SetZoomLevel(5.0f);
	}


	void EditorLayer::OnAttach()
	{		
		m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");


		Xaloc::FramebufferSpec framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		framebufferSpec.ClearColor = { 0.1f, 0.1f, 0.1f, 1 };
		Ref<Xaloc::Framebuffer> framebuffer = Xaloc::Framebuffer::Create(framebufferSpec);

		Xaloc::RenderPassSpecification renderPassSpec;
		renderPassSpec.TargetFramebuffer = framebuffer;
		m_RenderPass = Xaloc::RenderPass::Create(renderPassSpec);
		
		m_GuizmoRenderPass = Xaloc::RenderPass::Create(renderPassSpec);



		Xaloc::Ref<Xaloc::Texture2D> tilemap = Xaloc::Texture2D::Create("assets/game/textures/tilemap.png");

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

		Application::Get().GetImGuiLayer()->SetBlockEvents(!(m_ViewportFocused && m_ViewportHovered));
		if (m_ViewportFocused)
		{
			m_CameraController.OnUpdate(ts);
		}


		// RENDER

		Xaloc::Renderer2D::ResetStats();

		// TODO begin render pass
		//m_Framebuffer->Bind();
		//m_RenderPass->GetSpecification().TargetFramebuffer->Bind();
		Renderer::BeginRenderPass(m_RenderPass);

		//Xaloc::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		//Xaloc::RenderCommand::Clear();

		#if false   	
		Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());

		//Xaloc::Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f }, { 0.8f, 0.8f }, m_Rotation, m_SecondColor);
		Xaloc::Renderer2D::DrawQuad({ -0.5f, 0.5f, 1.0f }, { 0.8f, 0.8f }, m_SecondColor);
		Xaloc::Renderer2D::DrawQuad({ 0.25f, -0.25f }, { 1.2f, 1.2f }, m_FirstColor);
		Xaloc::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, m_TilingFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
		Xaloc::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, glm::radians(m_Rotation), m_Texture, 0.5f);

		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.5f };
				Xaloc::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}

		Xaloc::Renderer2D::EndScene();
		#endif


		Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());

		for (uint32_t y = 0; y < s_mapHeight; y++)
		{
			for (uint32_t x = 0; x < s_mapWidth; x++)
			{
				char tileType = s_MapTiles[((s_mapHeight - 1) - y) * s_mapWidth + x];

				glm::vec3 pos = { x - (s_mapWidth / 2.0f), y - (s_mapHeight / 2.0f), m_TilesDepth };
				glm::vec3 waterPos = { pos.x, pos.y, m_TilesDepth - 0.0001f };
				Xaloc::Renderer2D::DrawQuad(waterPos, { 1.0f, 1.0f }, m_TileWater);

				if (tileType == 'W')
					continue;

				if (s_TextureMap.find(tileType) != s_TextureMap.end())
				{
					Xaloc::Ref<Xaloc::SubTexture2D> tex = s_TextureMap[tileType];
					Xaloc::Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, tex);
				}
				else
				{
					Xaloc::Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
				}
			}
		}

		Xaloc::Renderer2D::EndScene();


		
		// UPDATE AND RENDER SCENE
		
		Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_Scene->OnUpdate(ts);
		Xaloc::Renderer2D::EndScene();



		// TODO end render pass
		//m_Framebuffer->Unbind();
		Renderer::EndRenderPass();

		
		// DRAW SELECTED ENTITIES GUI
		
		if (m_SelectionContext.size())
		{
			Entity selection = m_SelectionContext[0].Entity;

			Xaloc::Renderer::BeginRenderPass(m_GuizmoRenderPass, false);
			Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());

			// TODO get app pixels per unit
			float pxPerUnit = 16.0f;
			
			glm::vec4 sprMin = selection.Transform() * glm::vec4{ -0.5f - 1.0f / pxPerUnit, -0.5f - 1.0f / pxPerUnit, 0.0f, 1.0f };        // Get sprite quad min vertex
			glm::vec4 sprMax = selection.Transform() * glm::vec4{ 0.5f + 1.0f / pxPerUnit, 0.5f + 1.0f / pxPerUnit, 0.0f, 1.0f };          // Get sprite quad max vertex
			
			//glm::vec4 color = { 0.549f, 0.976f, 1.0f, 1.0f };
			glm::vec4 color = glm::vec4(1.0f);
			
			Renderer2D::DrawLine({ sprMin.x, sprMin.y }, { sprMax.x, sprMin.y }, color, 0.5f);
			Renderer2D::DrawLine({ sprMax.x, sprMin.y }, { sprMax.x, sprMax.y }, color, 0.5f);
			Renderer2D::DrawLine({ sprMax.x, sprMax.y }, { sprMin.x, sprMax.y }, color, 0.5f);
			Renderer2D::DrawLine({ sprMin.x, sprMax.y }, { sprMin.x, sprMin.y }, color, 0.5f);

			Xaloc::Renderer2D::EndScene();
			Xaloc::Renderer::EndRenderPass();
		}

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
				if (ImGui::MenuItem("Save"))
				{
					// TODO
				}
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
		

		
		// ============================================= VIEWPORT ============================================= //

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		
		auto viewportOffset = ImGui::GetCursorPos();
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		
		if (m_ViewportSize != *((glm::vec2*) & viewportSize))
		{
			m_RenderPass->GetSpecification().TargetFramebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			//m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
			m_ViewportSize = { viewportSize.x, viewportSize.y };

			// TODO OnWindow resize, call m_CameraController.OnResize again 
			m_CameraController.OnResize(viewportSize.x, viewportSize.y);
		}

		auto windowSize = ImGui::GetWindowSize();
		ImVec2 minBound = ImGui::GetWindowPos();
		minBound.x += viewportOffset.x;
		minBound.y += viewportOffset.y;

		ImVec2 maxBound = { minBound.x + windowSize.x, minBound.y + windowSize.y };

		m_ViewportBounds[0] = { minBound.x, minBound.y };
		m_ViewportBounds[1] = { maxBound.x, maxBound.y };

		// TODO m_AllowViewportCameraEvents = ImGui::IsMouseHoveringRect(minBound, maxBound);

		
		uint32_t texID = m_RenderPass->GetSpecification().TargetFramebuffer->GetColorAttachmentRendererID();
		//uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)texID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();


		ImGui::Begin("Viewport Info");
		ImGui::Text("Min bound: [%f, %f]", m_ViewportBounds[0].x, m_ViewportBounds[0].y);
		ImGui::Text("Max bound: [%f, %f]", m_ViewportBounds[1].x, m_ViewportBounds[1].y);
		ImGui::Separator();
		ImGui::Text("Window size: [%f, %f]", viewportSize.x, viewportSize.y);
		ImGui::End();
		

		// ============================================= GUIZMO ============================================= //

		if (m_SelectionContext.size())
		{
			// TODO

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
		m_CameraController.OnEvent(e);

		m_Scene->OnEvent(e);
		
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(XA_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
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
					boundingBox.Min = sprMin * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space
					boundingBox.Max = sprMax * m_CameraController.GetCamera().GetViewProjectionMatrix();  // Translate it to screen space

					XA_CORE_TRACE("AABB min ({0} , {1});  max ({2}, {3})", boundingBox.Min.x, boundingBox.Min.y, boundingBox.Max.x, boundingBox.Max.y);
					
					bool intersects = mouseX > boundingBox.Min.x && mouseX < boundingBox.Max.x
									&& mouseY > boundingBox.Min.y && mouseY < boundingBox.Max.y;

					// TODO don't intersect if alpha == 0
					
					if (intersects)
					{
						XA_CORE_WARN("INTERSECTION!");
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

	

	std::pair<float, float> EditorLayer::GetMouseViewportSpace()
	{
		auto [mx, my] = ImGui::GetMousePos(); // Input::GetMousePosition();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		auto viewportWidth = m_ViewportBounds[1].x - m_ViewportBounds[0].x;
		auto viewportHeight = m_ViewportBounds[1].y - m_ViewportBounds[0].y;

		return { (mx / viewportWidth) * 2.0f - 1.0f, ((my / viewportHeight) * 2.0f - 1.0f) * -1.0f };
	}


}
