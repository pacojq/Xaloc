#include <Xaloc.h>
#include "Xaloc/Core/EntryPoint.h"

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandboxLayer.h"


class ExampleLayer : public Xaloc::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true), m_SquarePosition(0.0f), m_SquareColor(0.2f, 0.3f, 0.8f)
	{
		// = = = = = = = = = = RENDERING A TRANGLE = = = = = = = = = = //


		m_VertexArray = Xaloc::VertexArray::Create();

		float vertices[3 * 7] = {
		   -0.5f, -0.5f, 0.0f,		0.8f, 0.0f, 0.2f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.2f, 0.3f, 0.8f, 1.0f,
			0.0f,  0.5f, 0.0f,		0.8f, 0.8f, 0.2f, 1.0f,
		};

		Xaloc::Ref<Xaloc::VertexBuffer> vertexBuffer = Xaloc::VertexBuffer::Create(vertices, sizeof(vertices));

		Xaloc::BufferLayout layout = {
			{ Xaloc::ShaderDataType::Float3, "a_Position" },
			{ Xaloc::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);


		uint32_t indices[3] = { 0, 1, 2 };
		Xaloc::Ref<Xaloc::IndexBuffer> indexBuffer = Xaloc::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);







		// = = = = = = = = = = RENDERING A SQUARE = = = = = = = = = = //

		m_SquareVertexArray = Xaloc::VertexArray::Create();

		float squareVertices[5 * 4] = {
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
		    0.5f, -0.5f, 0.0f,   1.0f, 0.0f,
			0.5f,  0.5f, 0.0f,   1.0f, 1.0f,
		   -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
		};

		Xaloc::Ref<Xaloc::VertexBuffer> squareVB = Xaloc::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Xaloc::ShaderDataType::Float3, "a_Position" },
			{ Xaloc::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Xaloc::Ref<Xaloc::IndexBuffer> squareIB = Xaloc::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVertexArray->SetIndexBuffer(squareIB);







		// = = = = = = = = = = = = = SHADERS = = = = = = = = = = = = = //

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 o_Color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				o_Color = vec4((v_Position + 1.0) * 0.5, 1.0);
				o_Color = v_Color;
			}
		)";

		m_ShaderLibrary.Add(Xaloc::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc));





		std::string flatColorVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 o_Color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				o_Color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Xaloc::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);





		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");


		m_Texture = Xaloc::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Xaloc::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Xaloc::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Xaloc::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

	}

	void OnUpdate(Xaloc::Timestep ts) override
	{
		// ================== UPDATE ================== //

		m_CameraController.OnUpdate(ts);

		if (Xaloc::Input::IsKeyPressed(XA_KEY_J))
			m_SquarePosition.x -= m_SquareMoveSpeed * ts;
		if (Xaloc::Input::IsKeyPressed(XA_KEY_L))
			m_SquarePosition.x += m_SquareMoveSpeed * ts;
		if (Xaloc::Input::IsKeyPressed(XA_KEY_I))
			m_SquarePosition.y += m_SquareMoveSpeed * ts;
		if (Xaloc::Input::IsKeyPressed(XA_KEY_K))
			m_SquarePosition.y -= m_SquareMoveSpeed * ts;


		// ================== RENDER ================== //

		Xaloc::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Xaloc::RenderCommand::Clear();

		Xaloc::Renderer::BeginScene(m_CameraController.GetCamera());



		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Xaloc::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Xaloc::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				glm::vec3 pos(i * 0.11f, j * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition + pos) * scale;
				Xaloc::Renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind(0);
		Xaloc::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind(0);
		Xaloc::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Xaloc::Renderer::Submit(m_Shader, m_VertexArray);



		Xaloc::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();

		ImGui::ShowDemoWindow();
	}

	void OnEvent(Xaloc::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}


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





class Sandbox : public Xaloc::Application
{
public:
	Sandbox(Xaloc::ApplicationSpec spec) : Xaloc::Application(spec)
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new SandboxLayer());
	}

	~Sandbox()
	{

	}
};


Xaloc::Application* Xaloc::CreateApplication()
{
	ApplicationSpec spec;

	spec.Name = "Sandbox App";
	spec.TargetGraphics = Xaloc::RendererAPI::API::OpenGL;
	//spec.PauseOnFocusLost = true;

	return new Sandbox(spec);
}