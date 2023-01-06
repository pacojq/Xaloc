#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/VertexBuffer.h"


namespace Xaloc {

	struct PipelineSpecification
	{
		Ref<Shader> Shader;
		VertexBufferLayout Layout;
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual void Invalidate() = 0;

		virtual void Bind() = 0;
		
		virtual const PipelineSpecification& GetSpecification() const = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};
}