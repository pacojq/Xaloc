#include "xapch.h"
#include "VulkanPhysicalDevice.h"

#include "../VulkanShared.h"
#include "../VulkanUtils.h"

#include <map>

namespace Xaloc {

	VulkanPhysicalDevice::VulkanPhysicalDevice()
	{
	}

	
	void VulkanPhysicalDevice::Create()
	{
		VkInstance vkInstance = VulkanShared::Resources().Instance;
		m_PhysicalDevice = VK_NULL_HANDLE;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

		XA_CORE_ASSERT(deviceCount > 0, "Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());


		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices)
		{
			int score = VulkanUtils::RateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0)
		{
			m_PhysicalDevice = candidates.rbegin()->second;
		}


		
		XA_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");

		// Get the final properties
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_Properties);
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_Features);
		vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &m_MemoryProperties);

		XA_CORE_INFO("    Physical device ready:");
		XA_CORE_INFO("        Device name: {0}", m_Properties.deviceName);
	}


	void VulkanPhysicalDevice::CleanUp()
	{
		
	}
	
}