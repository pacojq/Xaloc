#include "xapch.h"
#include "OpenGLPipeline.h"

#include "Xaloc/Renderer/Renderer.h"

#include <glad/glad.h>

namespace Xaloc {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;

		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;

		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;

		case ShaderDataType::Bool:		return GL_BOOL;
		}

		XA_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLPipeline::OpenGLPipeline(const PipelineSpecification& spec)
		: m_Specification(spec)
	{
		Invalidate();
	}

	OpenGLPipeline::~OpenGLPipeline()
	{
		GLuint rendererID = m_VertexArrayRendererID;
		glDeleteVertexArrays(1, &rendererID);
	}



	void OpenGLPipeline::Invalidate()
	{
		XA_CORE_ASSERT(m_Specification.Layout.GetElements().size(), "Layout is empty!");


		if (m_VertexArrayRendererID)
			glDeleteVertexArrays(1, &m_VertexArrayRendererID);

		glGenVertexArrays(1, &m_VertexArrayRendererID);
		glBindVertexArray(m_VertexArrayRendererID);

		// TODO Bind() ?
		
		glBindVertexArray(0);
	}

	

	void OpenGLPipeline::Bind()
	{
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
	}

}