#pragma once

#include <vulkan/vulkan.h>

namespace Xaloc {

	class VulkanPhysicalDevice
	{		
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice() = default;

		/// <summary>
		/// Picks a physical device.
		/// </summary>
		void Create();

		void CleanUp();


		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		
		
		// TODO bool IsExtensionSupported(const std::string& extensionName) const;


	private:
		VkPhysicalDevice m_PhysicalDevice;

		VkPhysicalDeviceProperties m_Properties;
		VkPhysicalDeviceFeatures m_Features;
		VkPhysicalDeviceMemoryProperties m_MemoryProperties;
		
		// TODO std::vector<VkQueueFamilyProperties> m_QueueFamilyProperties;
		// TODO std::unordered_set<std::string> m_SupportedExtensions;
	};


}