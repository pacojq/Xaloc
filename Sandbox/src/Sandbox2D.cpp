#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/type_ptr.hpp>


Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_CameraController(1280.0f / 720.0f, true),
	m_TilingFactor(1.0f),
	m_Rotation(0.0f),
	m_FirstColor(0.2f, 0.3f, 0.8f, 1.0f),
	m_SecondColor(0.8f, 0.2f, 0.3f, 1.0f)
{
	m_Scene = Xaloc::CreateRef<Xaloc::Scene>("Sandbox Scene");
}


void Sandbox2D::OnAttach()
{
	m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");


	Xaloc::GameObject* go = m_Scene->CreateGameObject("Player");
	
	Xaloc::SpriteRenderer* spr = new Xaloc::SpriteRenderer("assets/textures/Checkerboard.png");

	go->AddComponent(spr);
}

void Sandbox2D::OnDetach()
{
}


void Sandbox2D::OnUpdate(Xaloc::Timestep ts)
{
	// UPDATE

	m_CameraController.OnUpdate(ts);

	//m_Scene->OnUpdate(ts);



	// RENDER

	Xaloc::Renderer2D::ResetStats();

	Xaloc::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Xaloc::RenderCommand::Clear();

	Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());


	m_Scene->OnUpdate(ts);

	   	  
	//Xaloc::Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f }, { 0.8f, 0.8f }, m_Rotation, m_SecondColor);
	Xaloc::Renderer2D::DrawQuad({ -0.5f, 0.5f, 1.0f }, { 0.8f, 0.8f }, m_SecondColor);
	Xaloc::Renderer2D::DrawQuad({ 0.25f, -0.25f }, { 1.2f, 1.2f }, m_FirstColor);
	Xaloc::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, m_Texture, m_TilingFactor, { 1.0f, 1.0f, 1.0f, 1.0f });
	Xaloc::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 2.0f, 2.0f }, m_Rotation, m_Texture, 0.5f);

	//Xaloc::Renderer2D::EndScene();
	//Xaloc::Renderer2D::BeginScene(m_CameraController.GetCamera());

	for (float y = -5.0f; y < 5.0f; y += 0.5f)
	{
		for (float x = -5.0f; x < 5.0f; x += 0.5f)
		{
			glm::vec4 color = { (x + 5.0f) / 10.0f, (y + 5.0f) / 10.0f, 1.0f, 0.5f };
			Xaloc::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
		}
	}

	Xaloc::Renderer2D::EndScene();

}



void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::DragFloat("Tiling factor", &m_TilingFactor, 0.1f, 0.1f, 10.0f);
	ImGui::DragFloat("Rotation", &m_Rotation, 0.1f);
	ImGui::ColorEdit4("First Color", glm::value_ptr(m_FirstColor));
	ImGui::ColorEdit4("Second Color", glm::value_ptr(m_SecondColor));
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

void Sandbox2D::OnEvent(Xaloc::Event& e)
{
	m_CameraController.OnEvent(e);
}