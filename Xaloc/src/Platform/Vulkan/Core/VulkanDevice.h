#pragma once

#include <vulkan/vulkan.h>

namespace Xaloc {

	class VulkanDevice
	{
	public:
		VulkanDevice();
		~VulkanDevice() = default;

		void Create();
		void CleanUp();

		void CreateCommandPool();
		

		VkDevice GetDevice() const { return m_Device; }
		
		VkCommandPool GetCommandPool() const { return m_CommandPool; }
		
		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }
		VkQueue GetPresentQueue() const { return m_PresentQueue; }
		
	private:
		VkDevice m_Device;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkCommandPool m_CommandPool;

		
		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};


}