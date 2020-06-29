#pragma once

#include "Xaloc/Core/Core.h"

#include <glm/glm.hpp>

namespace Xaloc {

	struct FramebufferSpec
	{
		uint32_t Width;
		uint32_t Height;

		glm::vec4 ClearColor;
		
		// TODO FramebufferFormat Format;
		
		uint32_t Samples = 1;

		/// <summary>
		/// Whether we will render directly to the Swap Chain.
		/// </summary>
		bool SwapChainTarget = false;
	};

	
	class Framebuffer
	{
	public:
		static Ref<Framebuffer> Create(const FramebufferSpec& spec);

		virtual ~Framebuffer() = default;
		
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		virtual const FramebufferSpec& GetSpecification() const = 0;
	};
}