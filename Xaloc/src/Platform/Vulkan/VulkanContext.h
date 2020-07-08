#pragma once
#include "Xaloc/Renderer/GraphicsContext.h"

#include "VulkanSwapChain.h"

#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Xaloc {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual ~VulkanContext() override;

		virtual void Init() override;
		virtual void SwapBuffers() override;


	private:

		void CleanUp();
		
		void CreateInstance();
		void SetupDebugMessenger();
		
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		
		void CreateSwapChain();



		// UTIL FUNCTIONS
		
		bool CheckValidationLayerSupport() const;
		std::vector<const char*> GetRequiredExtensions() const;

		
	private:

		// How many frames should be processed concurrently.
		const int MAX_FRAMES_IN_FLIGHT = 2;

		const std::vector<const char*> m_DeviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};


	private:
		
		GLFWwindow* m_Window;
		VkInstance m_Instance;

		VkSurfaceKHR m_Surface;

		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;

		VkQueue m_GraphicsQueue;


		// Vulkan presentation queue
		
		VkQueue m_PresentQueue;
		size_t m_CurrentFrame = 0;


		// Swap chain

		Ref<VulkanSwapChain> m_SwapChain;

		

		
		// Validation and debugging

		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
		
		bool m_EnableValidationLayers;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}