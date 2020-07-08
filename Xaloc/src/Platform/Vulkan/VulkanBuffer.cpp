#include "xapch.h"
#include "VulkanBuffer.h"

namespace Xaloc {


	// = = = = = = = = = = = = = VERTEX BUFFER = = = = = = = = = = = = = //

	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size)
	{
		// TODO
	}

	VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t size)
	{
		// TODO
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		// TODO
	}

	void VulkanVertexBuffer::Bind() const
	{
		// TODO
	}

	void VulkanVertexBuffer::Unbind() const
	{
		// TODO
	}

	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		// TODO
	}





	// = = = = = = = = = = = = = INDEX BUFFER = = = = = = = = = = = = = //


	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		// TODO
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		// TODO
	}

	void VulkanIndexBuffer::Bind() const
	{
		// TODO
	}

	void VulkanIndexBuffer::Unbind() const
	{
		// TODO
	}

}