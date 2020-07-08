#pragma once

#include <vulkan/vulkan.h>

namespace Xaloc {

	struct VulkanSwapChainSpecification
	{
		VkDevice Device;
		VkPhysicalDevice PhysicalDevice;
		VkSurfaceKHR Surface;
		uint32_t Width;
		uint32_t Height;
	};

	
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain(const VulkanSwapChainSpecification& spec);
		
		void Recreate(const VulkanSwapChainSpecification& spec);
		void CleanUp(VkDevice device, VkSurfaceKHR surface);

		inline const VkSwapchainKHR& GetVulkanSwapChain() const { return m_SwapChain; }

		inline const VkFormat& GetImageFormat() const { return m_SwapChainImageFormat; }
		inline const VkExtent2D& GetExtent() const { return m_SwapChainExtent; }

		
	private:

		void CreateSelf(const VulkanSwapChainSpecification& spec);
		void CreateImageViews(VkDevice device);
		
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

		
	private:
		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;
	};
	
}