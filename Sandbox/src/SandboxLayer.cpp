#include "SandboxLayer.h"

#include "imgui/imgui.h"

#include "Random.h"

#include <glm/gtc/type_ptr.hpp>

// TODO temporary
#include "Xaloc/Scripting/ScriptEngine.h"

using namespace Xaloc;

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
		m_TilingFactor(1.0f),
		m_Rotation(0.0f),
		m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
		m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
{
	// Init Scene
	//m_Scene = CreateRef<Scene>("Sandbox Scene");	
	m_Scene = Scene::Load("assets/game/scenes/testScene.xaloc");
}


void SandboxLayer::OnAttach()
{	
	m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");


	Ref<Texture2D> tilemap = AssetManager::GetAsset<Texture2D>("assets/game/textures/tilemap.png");

	glm::vec2 size = { 16.0f, 16.0f };
	glm::vec2 pad = { 0.0f, 0.0f };
	glm::vec2 off = { 1.0f, 1.0f };

	s_TextureMap['G'] = SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 16.0f }, size, pad, off);

	s_TextureMap['T'] = SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 17.0f }, size, pad, off);
	s_TextureMap['B'] = SubTexture2D::CreateFromGrid(tilemap, { 1.0f, 15.0f }, size, pad, off);
	s_TextureMap['L'] = SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 16.0f }, size, pad, off);
	s_TextureMap['R'] = SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 16.0f }, size, pad, off);

	s_TextureMap['0'] = SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 15.0f }, size, pad, off);
	s_TextureMap['1'] = SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 15.0f }, size, pad, off);
	s_TextureMap['2'] = SubTexture2D::CreateFromGrid(tilemap, { 2.0f, 17.0f }, size, pad, off);
	s_TextureMap['3'] = SubTexture2D::CreateFromGrid(tilemap, { 0.0f, 17.0f }, size, pad, off);

	s_TextureMap['4'] = SubTexture2D::CreateFromGrid(tilemap, { 5.0f, 16.0f }, size, pad, off);
	s_TextureMap['5'] = SubTexture2D::CreateFromGrid(tilemap, { 6.0f, 16.0f }, size, pad, off);
	s_TextureMap['6'] = SubTexture2D::CreateFromGrid(tilemap, { 6.0f, 17.0f }, size, pad, off);
	s_TextureMap['7'] = SubTexture2D::CreateFromGrid(tilemap, { 5.0f, 17.0f }, size, pad, off);

	m_TileWater = SubTexture2D::CreateFromGrid(tilemap, { 9.0f, 10.0f }, size, pad, off);



	
	// CAMERA
	
	SceneCamera& mainCamera = m_Scene->GetCameraStack()->MainCamera();
	mainCamera.Camera.SetViewportSize(480, 270);
	mainCamera.Camera.SetClipDistance(-100, 100);


	

	// PLAYER

	//Ref<SubTexture2D> tilePlayer = SubTexture2D::CreateFromGrid(tilemap,
	//	{ 24.0f, 17.0f }, size, pad, off);

	m_Player = m_Scene->CreateEntity("Player");

	SpriteRendererComponent& playerSpr = m_Player.AddComponent<SpriteRendererComponent>();
	playerSpr.Sprite = AssetManager::GetMetadata("assets/game/textures/player.xaspr").Handle;

	m_Player.AddComponent<BehaviourComponent>("SandboxCs.PlayerEntity");

	// FOREST
	/*
	Entity* forest = m_Scene->CreateEntity("Forest");

	Ref<SubTexture2D> tree_0 = SubTexture2D::CreateFromGrid(tilemap, { 22.0f, 7.0f }, size, pad, off);
	Ref<SubTexture2D> tree_1 = SubTexture2D::CreateFromGrid(tilemap, { 22.0f, 8.0f }, size, pad, off);

	for (uint32_t y = 0; y < s_mapHeight; y++)
	{
		for (uint32_t x = 0; x < s_mapWidth; x++)
		{
			char tileType = s_MapTiles[((s_mapHeight - 1) - y) * s_mapWidth + x];
			glm::vec3 pos = { x - (s_mapWidth / 2.0f), y - (s_mapHeight / 2.0f), m_TilesDepth };

			if (tileType != 'G' || (x == s_mapWidth/2.0f && y == s_mapHeight/2.0f))
				continue;

			if (Random::Float() < 0.3f)
				continue;

			Ref<SubTexture2D> tex = Random::Float() < 0.5f ? tree_0 : tree_1;
			SpriteRenderer* sprite = new SpriteRenderer(tex);
			sprite->SetDepth(0.1f);
			sprite->SetLocalPosition(pos);
			forest->AddComponent(sprite);
		}
	}
	*/


	ScriptEngine::SetSceneContext(m_Scene); // TODO find a cleaner way to do this
	m_Scene->StartRuntime();
}

void SandboxLayer::OnDetach()
{
}


void SandboxLayer::OnUpdate(Timestep ts)
{
	Renderer2D::ResetStats();

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	#if false   	
	Renderer2D::BeginScene(m_CameraController.GetCamera());

	//Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f }, { 0.8f, 0.8f }, m_Rotation, m_SecondColor);
	Renderer2D::DrawQuad({ -0.5f, 0.5f, 1.0f }, { 0.8f, 0.8f }, m_SecondColor);
	Renderer2D::DrawQuad({ 0.25f, -0.25f }, { 1.2f, 1.2f }, m_FirstColor);
	Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, m_TilingFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, glm::radians(m_Rotation), m_Texture, 0.5f);

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.5f };
			Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

	Renderer2D::EndScene();
	#endif


	Renderer2D::BeginScene(m_Scene->GetCameraStack()->MainCamera().ViewProjectionMatrix());
	
	for (uint32_t y = 0; y < s_mapHeight; y++)
	{
		for (uint32_t x = 0; x < s_mapWidth; x++)
		{
			char tileType = s_MapTiles[((s_mapHeight - 1) - y) * s_mapWidth + x];

			glm::vec3 pos = { x - (s_mapWidth / 2.0f), y - (s_mapHeight / 2.0f), m_TilesDepth };
			glm::vec3 waterPos = { pos.x, pos.y, m_TilesDepth - 0.0001f };
			Renderer2D::DrawQuad(waterPos, { 1.0f, 1.0f }, m_TileWater);

			if (tileType == 'W')
				continue;
			
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
			{
				Ref<SubTexture2D> tex = s_TextureMap[tileType];
				Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, tex);
			}
			else
			{
				Renderer2D::DrawQuad(pos, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
			}
		}
	}
	
	Renderer2D::EndScene();


	Renderer2D::BeginScene(m_Scene->GetCameraStack()->MainCamera().ViewProjectionMatrix());
	m_Scene->OnUpdateRuntime(ts);
	Renderer2D::EndScene();
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

	//float depth = m_SpriteRenderer->GetDepth();
	//if (ImGui::DragFloat("SpriteRenderer depth", &depth, 0.1f, -10.0f, 10.0f))
	//{
	//	m_SpriteRenderer->SetDepth(depth);
	//}


	ImGui::Separator();
	ImGui::Text("Player data");

	glm::mat4 matrix = m_Player.GetComponent<TransformComponent>().GetTransform();
	glm::vec3 translation = {matrix[3][0], matrix[3][1], matrix[3][2] };

	//glm::vec3 playerPos = m_Player->GetPosition();
	ImGui::DragFloat("X", &translation.x, 0.1f);
	ImGui::DragFloat("Y", &translation.y, 0.1f);
	ImGui::DragFloat("Z", &translation.z, 0.1f);
	//m_Player->SetPosition(playerPos);

	//matrix[0][3] = translation.x;
	//matrix[1][3] = translation.y; 
	//matrix[2][3] = translation.z;
	matrix[3][0] = translation.x;
	matrix[3][1] = translation.y;
	matrix[3][2] = translation.z;

	m_Player.SetTransform(&matrix);

	//float playerSpd = m_PlayerComponent->GetSpeed();
	//ImGui::DragFloat("Speed", &playerSpd, 0.1f);
	//m_PlayerComponent->SetSpeed(playerSpd);

	ImGui::End();


	auto stats = Renderer2D::GetStats();

	ImGui::Begin("Render Stats");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::End();
	
	//ImGui::ShowDemoWindow();
}

void SandboxLayer::OnEvent(Event& e)
{
	// Do nothing (?)
}