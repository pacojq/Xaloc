#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Renderer/Buffer.h"
#include "Xaloc/Renderer/Shader.h"
#include "Xaloc/Renderer/RenderPass.h"


namespace Xaloc {

	struct PipelineSpecification
	{
		Ref<Shader> Shader;
		BufferLayout VertexBufferLayout;
		Ref<RenderPass> RenderPass; // TODO maybe this shouldn't be here
	};

	class Pipeline
	{
	public:
		virtual ~Pipeline() = default;

		virtual const PipelineSpecification& GetSpecification() const = 0;

		static Ref<Pipeline> Create(const PipelineSpecification& spec);
	};
}