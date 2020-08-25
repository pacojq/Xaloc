#include "xapch.h"
#include "Renderer.h"

#include "Pipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Renderer2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Xaloc {

	Scope<Renderer::SceneData> Renderer::m_SceneData = CreateScope<Renderer::SceneData>();

	struct RendererData
	{
		Ref<RenderPass> m_ActiveRenderPass;
		// TODO Ref<ShaderLibrary> m_ShaderLibrary;
		// TODO Ref<VertexArray> m_FullscreenQuadVertexArray;
	};

	static RendererData s_Data;

	

	void Renderer::Init()
	{		
		RenderCommand::Init(GetAPI());
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer2D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}


	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::BeginRenderPass(Ref<RenderPass> renderPass, bool clear)
	{
		XA_CORE_ASSERT(renderPass, "Render pass cannot be null!");

		s_Data.m_ActiveRenderPass = renderPass;

		renderPass->GetSpecification().TargetFramebuffer->Bind();
		if (clear)
		{
			const glm::vec4& clearColor = renderPass->GetSpecification().TargetFramebuffer->GetSpecification().ClearColor;
			RenderCommand::SetClearColor(clearColor);
			RenderCommand::Clear();
		}
	}

	void Renderer::EndRenderPass()
	{
		XA_CORE_ASSERT(s_Data.m_ActiveRenderPass, "No active render pass!");
		s_Data.m_ActiveRenderPass->GetSpecification().TargetFramebuffer->Unbind();
		s_Data.m_ActiveRenderPass = nullptr;
	}

	/* TODO 
	void Renderer::Submit(const Ref<Pipeline> pipeline, const Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> vertexBuffer, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
	
		vertexBuffer->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
	*/
}