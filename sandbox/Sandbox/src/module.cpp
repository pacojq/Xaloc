#include "module.h"
#include "Random.h"
#include "tooling/imgui/core.h"
#include "xaloc/core/assets/manager.h"
#include "xaloc/scripting/engine.h" // TODO temporary
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/renderer2D.h"
#include "xaloc/renderer/debugging/renderer2D.h"
#include <glm/gtc/type_ptr.hpp>

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

ZModuleSandbox::ZModuleSandbox()
		: Xaloc::ZModule("Sandbox 2D"),
		m_TilingFactor(1.0f),
		m_Rotation(0.0f),
		m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
		m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
{
	// Init Scene
	//m_Scene = CreateRef<Scene>("Sandbox Scene");	
	m_Scene = ZScene::Load("assets/game/scenes/testScene.xaloc");
}


void ZModuleSandbox::OnAttach()
{	
	m_Texture = ZTexture2D::Create("assets/textures/Checkerboard.png");

	ZRef<ZTexture2D> tilemap = ZAssetManager::GetAsset<ZTexture2D>("assets/game/textures/tilemap.png");

	glm::vec2 size = { 16.0f, 16.0f };
	glm::vec2 pad = { 0.0f, 0.0f };
	glm::vec2 off = { 1.0f, 1.0f };

	s_TextureMap['G'] = ZSubTexture2D::CreateFromGrid(tilemap, { 1.0f, 16.0f }, size, pad, off);

	s_TextureMap['T'] = ZSubTexture2D::CreateFromGrid(tilemap, { 1.0f, 17.0f }, size, pad, off);
	s_TextureMap['B'] = ZSubTexture2D::CreateFromGrid(tilemap, { 1.0f, 15.0f }, size, pad, off);
	s_TextureMap['L'] = ZSubTexture2D::CreateFromGrid(tilemap, { 0.0f, 16.0f }, size, pad, off);
	s_TextureMap['R'] = ZSubTexture2D::CreateFromGrid(tilemap, { 2.0f, 16.0f }, size, pad, off);

	s_TextureMap['0'] = ZSubTexture2D::CreateFromGrid(tilemap, { 0.0f, 15.0f }, size, pad, off);
	s_TextureMap['1'] = ZSubTexture2D::CreateFromGrid(tilemap, { 2.0f, 15.0f }, size, pad, off);
	s_TextureMap['2'] = ZSubTexture2D::CreateFromGrid(tilemap, { 2.0f, 17.0f }, size, pad, off);
	s_TextureMap['3'] = ZSubTexture2D::CreateFromGrid(tilemap, { 0.0f, 17.0f }, size, pad, off);

	s_TextureMap['4'] = ZSubTexture2D::CreateFromGrid(tilemap, { 5.0f, 16.0f }, size, pad, off);
	s_TextureMap['5'] = ZSubTexture2D::CreateFromGrid(tilemap, { 6.0f, 16.0f }, size, pad, off);
	s_TextureMap['6'] = ZSubTexture2D::CreateFromGrid(tilemap, { 6.0f, 17.0f }, size, pad, off);
	s_TextureMap['7'] = ZSubTexture2D::CreateFromGrid(tilemap, { 5.0f, 17.0f }, size, pad, off);

	m_TileWater = ZSubTexture2D::CreateFromGrid(tilemap, { 9.0f, 10.0f }, size, pad, off);

	// CAMERA
	
	SSceneCamera& rMainCamera = m_Scene->GetCameraStack()->MainCamera();
	rMainCamera.Camera.SetPixelSize(480, 270);
	rMainCamera.Camera.SetClipDistance(-100, 100);

	// PLAYER

	//ZRef<SubTexture2D> tilePlayer = SubTexture2D::CreateFromGrid(tilemap,
	//	{ 24.0f, 17.0f }, size, pad, off);

	m_Player = m_Scene->CreateEntity("Player");

	CmpSpriteRenderer& playerSpr = m_Player.AddComponent<CmpSpriteRenderer>();
	playerSpr.Sprite = ZAssetManager::ReadMetadata("assets/game/textures/player.xaspr").nHandle;

	m_Player.AddComponent<CmpBehaviour>("SandboxCs.PlayerEntity");

	// FOREST
	/*
	ZEntity* forest = m_Scene->CreateEntity("Forest");

	ZRef<SubTexture2D> tree_0 = SubTexture2D::CreateFromGrid(tilemap, { 22.0f, 7.0f }, size, pad, off);
	ZRef<SubTexture2D> tree_1 = SubTexture2D::CreateFromGrid(tilemap, { 22.0f, 8.0f }, size, pad, off);

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

			ZRef<SubTexture2D> tex = Random::Float() < 0.5f ? tree_0 : tree_1;
			SpriteRenderer* sprite = new SpriteRenderer(tex);
			sprite->SetDepth(0.1f);
			sprite->SetLocalPosition(pos);
			forest->AddComponent(sprite);
		}
	}
	*/

	Scripting::ZScriptEngine::SetSceneContext(m_Scene); // TODO find a cleaner way to do this
	m_Scene->StartRuntime();
}

void ZModuleSandbox::OnDetach()
{
}

void ZModuleSandbox::OnUpdate(ZTimestep ts)
{
#if RENDER_STATS_ENABLED
	ZDebugRenderer2D* pRenderDebug = ZRenderer2D::GetDebug();
	if (pRenderDebug)
	{
		pRenderDebug->ResetStats();
	}
#endif

	RendererGet()->SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RendererGet()->Clear();

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

	ZRenderer2D::BeginScene(m_Scene->GetCameraStack()->MainCamera().ViewProjectionMatrix());
	
	for (uint32_t y = 0; y < s_mapHeight; y++)
	{
		for (uint32_t x = 0; x < s_mapWidth; x++)
		{
			char tileType = s_MapTiles[((s_mapHeight - 1) - y) * s_mapWidth + x];

			glm::vec3 pos = { x - (s_mapWidth / 2.0f), y - (s_mapHeight / 2.0f), m_TilesDepth };
			glm::vec3 waterPos = { pos.x, pos.y, m_TilesDepth - 0.0001f };
			ZRenderer2D::DrawQuad(waterPos, { 1.0f, 1.0f }, m_TileWater);

			if (tileType == 'W')
				continue;
			
			if (s_TextureMap.find(tileType) != s_TextureMap.end())
			{
				ZRef<ZSubTexture2D> tex = s_TextureMap[tileType];
				ZRenderer2D::DrawQuad(pos, { 1.0f, 1.0f }, tex);
			}
			else
			{
				ZRenderer2D::DrawQuad(pos, { 1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f, 1.0f });
			}
		}
	}
	
	ZRenderer2D::EndScene();

	ZRenderer2D::BeginScene(m_Scene->GetCameraStack()->MainCamera().ViewProjectionMatrix());
	m_Scene->OnUpdateRuntime(ts);
	ZRenderer2D::EndScene();
}

void ZModuleSandbox::OnImGuiRender()
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

	CmpTransform& rTransform = m_Player.GetComponent<CmpTransform>();
	glm::mat4 matrix = rTransform.GetTransform();
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

	rTransform.SetTransform(matrix);

	//float playerSpd = m_PlayerComponent->GetSpeed();
	//ImGui::DragFloat("Speed", &playerSpd, 0.1f);
	//m_PlayerComponent->SetSpeed(playerSpd);

	ImGui::End();

#if RENDER_STATS_ENABLED
	const ZDebugRenderer2D* pRenderDebug = ZRenderer2D::GetDebug();
	if (pRenderDebug)
	{
		const SStatsRenderer2D& stats = pRenderDebug->ReadStats();
		ImGui::Begin("Render Stats");
		ImGui::Text("Draw Calls: %d", stats.nDrawCalls);
		ImGui::Text("Quads: %d", stats.nQuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();
	}
#endif
	
	//ImGui::ShowDemoWindow();
}

void ZModuleSandbox::OnEvent(ZEvent& rEvent)
{
	// Do nothing (?)
}