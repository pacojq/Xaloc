#include "xapch.h"
#include "VulkanDevice.h"

#include "../VulkanShared.h"
#include "../VulkanUtils.h"

#include <set>

namespace Xaloc {

	VulkanDevice::VulkanDevice()
	{
	}


	void VulkanDevice::Create()
	{
		auto context = VulkanShared::Resources().Context;
		VkPhysicalDevice physicalDevice = VulkanShared::Resources().PhysicalDevice->GetPhysicalDevice();


		
		QueueFamilyIndices indices = VulkanUtils::FindQueueFamilies(physicalDevice, context->GetSurface());

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

		if (VulkanShared::Resources().EnableValidationLayers)
		{
			auto validationLayers = VulkanShared::Resources().ValidationLayers;
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Device), "Failed to create logical device!");
		vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &m_PresentQueue);

		XA_CORE_TRACE("    Logical device ready");
	}



	
	void VulkanDevice::CleanUp()
	{
		vkDestroyDevice(m_Device, nullptr);
	}






	void VulkanDevice::CreateCommandPool()
	{
		auto context = VulkanShared::Resources().Context;
		VkPhysicalDevice physicalDevice = VulkanShared::Resources().PhysicalDevice->GetPhysicalDevice();

		QueueFamilyIndices queueFamilyIndices = VulkanUtils::FindQueueFamilies(physicalDevice, context->GetSurface());

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();
		poolInfo.flags = 0; // Optional

		VK_CHECK_RESULT(vkCreateCommandPool(m_Device, &poolInfo, nullptr, &m_CommandPool), "Failed to create command pool!");

		XA_CORE_TRACE("    Command pool ready");
	}

}