#pragma once

#include "Xaloc/Renderer/Pipeline.h"

namespace Xaloc {

	class DirectXPipeline : public Pipeline
	{
	public:
		DirectXPipeline(const PipelineSpecification& spec);
		virtual ~DirectXPipeline();

		virtual PipelineSpecification& GetSpecification() { return m_Specification; }
		virtual const PipelineSpecification& GetSpecification() const { return m_Specification; }

		virtual void Invalidate() override;

		virtual void Bind() override;
	private:
		PipelineSpecification m_Specification;
		uint32_t m_VertexArrayRendererID = 0;
	};

}