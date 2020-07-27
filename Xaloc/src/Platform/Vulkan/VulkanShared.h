#pragma once

#include <vulkan/vulkan.h>

#include "VulkanContext.h"

namespace Xaloc {

	struct VulkanResources
	{
		VkInstance Instance;

		VkSurfaceKHR Surface;

		VkDevice Device;
		VkPhysicalDevice PhysicalDevice;

		VulkanContext* Context;

		Ref<VulkanSwapChain> SwapChain;
	};


	class VulkanShared
	{
	public:
		static VulkanResources& Resources() { return s_Resources; }
	private:
		static VulkanResources s_Resources;
	};

}
