#pragma once

#include "Xaloc/Renderer/Buffer.h"

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

	

	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(uint32_t size);
		VulkanVertexBuffer(float* vertices, uint32_t size);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }

	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;

		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
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