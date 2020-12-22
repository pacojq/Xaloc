#pragma once

#include <vulkan/vulkan.h>

#include "VulkanContext.h"

namespace Xaloc {

	struct VulkanResources
	{
		VkInstance Instance;

		VkSurfaceKHR Surface;

		Ref <VulkanDevice> Device;
		Ref<VulkanPhysicalDevice> PhysicalDevice;

		VulkanContext* Context;

		Ref<VulkanSwapChain> SwapChain;


		
		// Validation and Debugging

		std::vector<const char*> ValidationLayers;
		bool EnableValidationLayers;
		
	};


	class VulkanShared
	{
	public:
		static VulkanResources& Resources() { return s_Resources; }
	private:
		static VulkanResources s_Resources;
	};

}
