#pragma once

#include "Xaloc.h"

class Sandbox2D : public Xaloc::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Xaloc::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Xaloc::Event& e) override;

private:

	Xaloc::OrthographicCameraController m_CameraController;

	// TEMP
	Xaloc::Ref<Xaloc::VertexArray> m_SquareVertexArray;
	Xaloc::Ref<Xaloc::Shader> m_FlatColorShader;

	float m_TilingFactor;
	float m_Rotation;
	glm::vec4 m_FirstColor;
	glm::vec4 m_SecondColor;
	Xaloc::Ref<Xaloc::Texture2D> m_Texture;

};