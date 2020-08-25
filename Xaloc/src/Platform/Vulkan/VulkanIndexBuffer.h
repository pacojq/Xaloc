#pragma once

#include "Xaloc/Renderer/IndexBuffer.h"

#include <vulkan/vulkan.h>

namespace Xaloc {

	class VulkanBuffer
	{
	public:

		static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

		static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);


		static VkCommandBuffer BeginSingleTimeCommands();

		static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	};


	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_RendererID;
		uint32_t m_Count;

		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
	};

}