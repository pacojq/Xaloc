#include "EditorLayer.h"

#include "Xaloc/Scripting/ScriptEngine.h"

#include "imgui/imgui.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#include <glm/gtc/type_ptr.hpp>


namespace Xaloc {


	EditorLayer::EditorLayer() : Layer("Editor Layer")
	{
		// Load Assets

		AssetManager::LoadTexture("TILEMAP", "assets/game/textures/tilemap.png");
		

		// Init Scene

		m_Scene = Scene::Load("assets/scenes/serializedScene.xaloc");

		m_MainCamera = m_Scene->CreateEntity("Orthographic Camera");
		
		auto& orthoData = m_MainCamera.AddComponent<CameraComponent>();
		orthoData.Camera.SetViewportSize(1280.0, 720.0);
		orthoData.Camera.SetProjectionType(SceneCamera::ProjectionType::Orthographic);
		orthoData.Camera.SetOrthographicNearClip(-100);
		orthoData.Camera.SetOrthographicFarClip(100);
		
		//ScriptEngine::SetSceneContext(m_Scene);
		//m_Scene->StartRuntime();

		

		
		// Init Editor

		m_EditorCamera = EditorCamera(glm::perspective(45.0f, 1.778f, 0.01f, 1000.0f));
		
		//m_Scene = Xaloc::CreateRef<Scene>("Sandbox Scene");
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_Scene);
		m_AssetManagerPanel = CreateScope<AssetManagerPanel>();

		m_GameViewport = CreateRef<EditorViewport>("Game Preview");
		m_SceneViewport = CreateRef<EditorViewport>("Scene");
		
		//m_CameraController.SetZoomLevel(5.0f);
	}


	void EditorLayer::OnAttach()
	{
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

	}

	void EditorLayer::OnDetach()
	{
	}


	void EditorLayer::OnUpdate(Timestep ts)
	{
		// UPDATE

		// TODO block events just to the viewport that doesn't need them

		bool blockEvents = true;

		if (m_GameViewport->IsFocused() && m_GameViewport->IsHovered())
		{
			blockEvents = false;
		}
		if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
		{
			blockEvents = false;
		}
		
		Application::Get().GetImGuiLayer()->SetBlockEvents(blockEvents);
		if (m_GameViewport->IsFocused())
		{
			// TODO update editor camera
			//m_CameraController.OnUpdate(ts);
		}
		

		// RENDER

		Xaloc::Renderer2D::ResetStats();


		// UPDATE AND RENDER SCENE
		
		Renderer::BeginRenderPass(m_RenderPass);
		m_Scene->OnUpdateEditor(ts, m_EditorCamera);
		Renderer::EndRenderPass();


		
		// RENDER SCENE FROM EDITOR CAMERA

		Renderer::BeginRenderPass(m_EditorRenderPass);
		m_Scene->OnUpdateEditor(ts, m_EditorCamera);
		//m_Scene->RenderScene(editorCamera, m_EditorCamera->GetTransform().GetTransform());
		Renderer::EndRenderPass();

		if (m_SceneViewport->IsFocused())
		{
			m_EditorCamera.OnUpdate(ts);
		}


		
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

		// TODO Play / Pause

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
		


		// ============================================= HIERARCHY ============================================= //

		m_SceneHierarchyPanel->OnImGuiRender();
		

		m_AssetManagerPanel->OnImGuiRender();
		
		
		// ============================================= VIEWPORTS ============================================= //

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


		m_SceneViewport->Begin();
		if (m_SceneViewport->Render(m_EditorRenderPass))
		{
			/*
			SceneCamera* camera = m_EditorCamera.GetCamera();

			uint32_t width = (uint32_t)m_SceneViewport->GetSize().x;
			uint32_t height = (uint32_t)m_SceneViewport->GetSize().y;
			camera->SetViewportSize(width, height);
			*/
		}


		

		// ============================================= GUIZMO ============================================= //

		/* TODO guizmo
		auto [camTranslation, camRotationQuat, camScale] = GetTransformDecomposition(m_EditorCamera->GetTransform().Transform);
		
		auto& viewMat = glm::translate(glm::mat4(1.0f), camTranslation) * glm::toMat4(camRotationQuat);
		viewMat = glm::inverse(viewMat);
		
		float rw = (float)ImGui::GetWindowWidth();
		float rh = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
		*/
		// TODO guizmo grid ImGuizmo::DrawGrid(glm::value_ptr(viewMat), glm::value_ptr(m_EditorCamera->GetCamera()->GetProjection()), glm::value_ptr(glm::mat4(1.0f)), 10.0f);
		
		if (m_SelectionContext.size())
		{
			auto& selection = m_SelectionContext[0];
			
			// TODO use camera controller
			//auto& camera = m_CameraController.GetCamera();
			/*
			bool snap = false; // TODO Input::IsKeyPressed(XA_KEY_LEFT_CONTROL);

			auto& entityTransform = selection.Entity.Transform();
			
			// TODOfloat snapValue[3] = { m_SnapValue, m_SnapValue, m_SnapValue };
			float snapValue[3] = { 0.1, 0.1, 0.1 };
			
			//if (m_SelectionMode == SelectionMode::Entity)
			{
				ImGuizmo::Manipulate(glm::value_ptr(viewMat),
					glm::value_ptr(m_EditorCamera->GetCamera()->GetProjection()),
					(ImGuizmo::OPERATION)ImGuizmo::OPERATION::TRANSLATE,
					ImGuizmo::LOCAL,
					glm::value_ptr(entityTransform),
					nullptr,
					nullptr);
			}
			*/
		}
		
		m_SceneViewport->End();
		
		ImGui::End();

	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Scene->OnEvent(e);

		if (m_SceneViewport->IsFocused() && m_SceneViewport->IsHovered())
		{
			m_EditorCamera.OnEvent(e);
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
		m_EditorCamera.SetViewportSize(width, height);
		
		return false;
	}


	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		auto [mx, my] = Input::GetMousePosition();
		if (e.GetMouseButton() == XA_MOUSE_BUTTON_LEFT) // TODO && !Input::IsKeyPressed(KeyCode::LeftAlt) && !ImGuizmo::IsOver())
		{
			auto [mouseX, mouseY] = m_SceneViewport->GetMouseViewportSpace();
			if (m_GameViewport->IsFocused() && m_GameViewport->IsHovered())
			{
				auto [mx, my] = m_GameViewport->GetMouseViewportSpace();
				mouseX = mx;
				mouseY = my;
			}
			
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

	


}
