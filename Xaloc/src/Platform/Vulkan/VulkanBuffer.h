#pragma once

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


}