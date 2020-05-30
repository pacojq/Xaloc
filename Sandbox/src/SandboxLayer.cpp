#include "SandboxLayer.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>

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



SandboxLayer::SandboxLayer()
	: Layer("Sandbox 2D"),
	m_CameraController(1280.0f / 720.0f, true),
	m_TilingFactor(1.0f),
	m_Rotation(0.0f),
	m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
	m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
{
	m_Scene = Xaloc::CreateRef<Xaloc::Scene>("Sandbox Scene");
	m_CameraController.SetZoomLevel(5.0f);
}


void SandboxLayer::OnAttach()
{
	m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");

	Xaloc::Ref<Xaloc::Texture2D> tilemap = Xaloc::Texture2D::Create("assets/game/textures/tilemap.png");
	m_TextureStairs = Xaloc::SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 5.0f }, { 16.0f, 16.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f });


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


	Xaloc::GameObject* go = m_Scene->CreateGameObject("Player");
	m_SpriteRenderer = new Xaloc::SpriteRenderer(m_TextureStairs);
	go->AddComponent(m_SpriteRenderer);
}

void SandboxLayer::OnDetach()
{
}


void SandboxLayer::OnUpdate(Xaloc::Timestep ts)
{
	// UPDATE

	m_CameraController.OnUpdate(ts);

	//m_Scene->OnUpdate(ts);



	// RENDER

	Xaloc::Renderer2D::ResetStats();

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
			
			Xaloc::Ref<Xaloc::SubTexture2D> tex;

			if (s_TextureMap.find(tileType) != s_TextureMap.end())
				tex = s_TextureMap[tileType];
			else tex = m_TextureStairs;

			Xaloc::Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, tex);
		}
	}

	Xaloc::Renderer2D::EndScene();


	Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());
	m_Scene->OnUpdate(ts);
	Xaloc::Renderer2D::EndScene();

}



void SandboxLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::DragFloat("Tiling factor", &m_TilingFactor, 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("Rotation", &m_Rotation, 0.1f);
	ImGui::ColorEdit4("First Color", glm::value_ptr(m_FirstColor));
	ImGui::ColorEdit4("Second Color", glm::value_ptr(m_SecondColor));

	ImGui::Separator();

	ImGui::DragFloat("Tiles depth", &m_TilesDepth, 0.1f, -10.0f, 10.0f);

	float depth = m_SpriteRenderer->GetDepth();
	if (ImGui::DragFloat("SpriteRenderer depth", &depth, 0.1f, -10.0f, 10.0f))
	{
		m_SpriteRenderer->SetDepth(depth);
	}

	ImGui::End();


	auto stats = Xaloc::Renderer2D::GetStats();

	ImGui::Begin("Render Stats");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
	
	//ImGui::ShowDemoWindow();
}

void SandboxLayer::OnEvent(Xaloc::Event& e)
{
	m_CameraController.OnEvent(e);
}