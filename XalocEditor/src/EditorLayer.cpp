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
		: Layer("Sandbox 2D"),
		m_CameraController(1280.0f / 720.0f, true),
		m_TilingFactor(1.0f),
		m_Rotation(0.0f),
		m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
		m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
	{
		m_Scene = Xaloc::CreateRef<Xaloc::Scene>("Sandbox Scene");
		m_SceneHierarchyPanel = CreateScope<SceneHierarchyPanel>(m_Scene);
		
		m_CameraController.SetZoomLevel(5.0f);
	}


	void EditorLayer::OnAttach()
	{
		m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");


		Xaloc::FramebufferSpec framebufferSpec;
		framebufferSpec.Width = 1280;
		framebufferSpec.Height = 720;
		m_Framebuffer = Xaloc::Framebuffer::Create(framebufferSpec);





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



		// PLAYER

		Xaloc::Ref<Xaloc::SubTexture2D> tilePlayer = Xaloc::SubTexture2D::CreateFromGrid(tilemap,
			{ 24.0f, 17.0f }, size, pad, off);

		m_Player = m_Scene->CreateEntity("Player");
		m_Player.AddComponent<Xaloc::SpriteRendererComponent>(tilePlayer);
		m_Player.AddComponent<Xaloc::BehaviourComponent>("SandboxCs.PlayerEntity");

	}

	void EditorLayer::OnDetach()
	{
	}


	void EditorLayer::OnUpdate(Xaloc::Timestep ts)
	{
		// UPDATE

		m_CameraController.OnUpdate(ts);



		// RENDER

		Xaloc::Renderer2D::ResetStats();

		m_Framebuffer->Bind();

		Xaloc::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Xaloc::RenderCommand::Clear();

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


		Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_Scene->OnUpdate(ts);
		Xaloc::Renderer2D::EndScene();

		m_Framebuffer->Unbind();
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Xaloc"))
			{
				if (ImGui::MenuItem("Exit")) Xaloc::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}







		

		ImGui::Begin("Settings");
		ImGui::DragFloat("Tiling factor", &m_TilingFactor, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Rotation", &m_Rotation, 0.1f);
		ImGui::ColorEdit4("First Color", glm::value_ptr(m_FirstColor));
		ImGui::ColorEdit4("Second Color", glm::value_ptr(m_SecondColor));

		ImGui::Separator();

		ImGui::DragFloat("Tiles depth", &m_TilesDepth, 0.1f, -10.0f, 10.0f);

		//float depth = m_SpriteRenderer->GetDepth();
		//if (ImGui::DragFloat("SpriteRenderer depth", &depth, 0.1f, -10.0f, 10.0f))
		//{
		//	m_SpriteRenderer->SetDepth(depth);
		//}


		ImGui::Separator();
		ImGui::Text("Player data");

		glm::mat4 matrix = m_Player.Transform();
		glm::vec3 translation = { matrix[3][0], matrix[3][1], matrix[3][2] };

		ImGui::DragFloat("X", &translation.x, 0.1f);
		ImGui::DragFloat("Y", &translation.y, 0.1f);
		ImGui::DragFloat("Z", &translation.z, 0.1f);

		matrix[3][0] = translation.x;
		matrix[3][1] = translation.y;
		matrix[3][2] = translation.z;

		m_Player.SetTransform(&matrix);


		ImGui::End();


		auto stats = Xaloc::Renderer2D::GetStats();

		ImGui::Begin("Render Stats");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();

		//ImGui::ShowDemoWindow();










		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Scene");
		ImVec2 scenePanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize != *((glm::vec2*) &scenePanelSize))
		{
			m_Framebuffer->Resize((uint32_t)scenePanelSize.x, (uint32_t)scenePanelSize.y);
			m_ViewportSize = { scenePanelSize .x, scenePanelSize .y };

			// TODO OnWindow resize, call m_CameraController.OnResize again 
			m_CameraController.OnResize(scenePanelSize.x, scenePanelSize.y);
		}
		
		uint32_t texID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)texID, scenePanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();


		m_SceneHierarchyPanel->OnImGuiRender();
		

		ImGui::End();

	}

	void EditorLayer::OnEvent(Xaloc::Event& e)
	{
		m_CameraController.OnEvent(e);
	}


}