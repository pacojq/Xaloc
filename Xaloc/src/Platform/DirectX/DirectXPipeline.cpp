#include "xapch.h"
#include "DirectXPipeline.h"

#include "Xaloc/Renderer/Renderer.h"

#include <d3d11.h>
#include <wrl.h>

namespace Xaloc {

	static DXGI_FORMAT ShaderDataTypeToDirectXBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;

		case ShaderDataType::Mat3:		return DXGI_FORMAT_UNKNOWN;
		case ShaderDataType::Mat4:		return DXGI_FORMAT_UNKNOWN;

		case ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
		case ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;

		case ShaderDataType::Bool:		return DXGI_FORMAT_R8_TYPELESS;
		}

		XA_CORE_ASSERT(false, "Unknown ShaderDataType");
		return DXGI_FORMAT_UNKNOWN;
	}

	DirectXPipeline::DirectXPipeline(const PipelineSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	DirectXPipeline::~DirectXPipeline()
	{
		/*
		GLuint rendererID = m_VertexArrayRendererID;
		glDeleteVertexArrays(1, &rendererID);
		*/
	}



	void DirectXPipeline::Invalidate()
	{
		/*
		XA_CORE_ASSERT(m_Specification.Layout.GetElements().size(), "Layout is empty!");


		if (m_VertexArrayRendererID)
			glDeleteVertexArrays(1, &m_VertexArrayRendererID);

		glGenVertexArrays(1, &m_VertexArrayRendererID);
		glBindVertexArray(m_VertexArrayRendererID);

		// TODO Bind() ?
		
		glBindVertexArray(0);
		*/
	}

	

	void DirectXPipeline::Bind()
	{
		/*
		XA_CORE_ASSERT(m_Specification.Layout.GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(m_VertexArrayRendererID);
		//vertexBuffer->Bind();

		const auto& layout = m_Specification.Layout;

		uint32_t attribIndex = 0;
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(
				attribIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.Offset
			);
			attribIndex++;
		}
		*/
	}

}