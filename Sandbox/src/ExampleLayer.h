#pragma once

#include "Xaloc.h"

class ExampleLayer : public Xaloc::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	void OnUpdate(Xaloc::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Xaloc::Event & e) override;


private:
	Xaloc::ShaderLibrary m_ShaderLibrary;

	Xaloc::Ref<Xaloc::VertexArray> m_VertexArray;

	Xaloc::Ref<Xaloc::VertexArray> m_SquareVertexArray;
	Xaloc::Ref<Xaloc::Shader> m_FlatColorShader;

	Xaloc::Ref<Xaloc::Texture2D> m_Texture;
	Xaloc::Ref<Xaloc::Texture2D> m_ChernoLogoTexture;

	Xaloc::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquarePosition;
	float m_SquareMoveSpeed = 3.0f;

	glm::vec3 m_SquareColor;
};
