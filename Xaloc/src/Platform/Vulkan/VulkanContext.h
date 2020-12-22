#pragma once
#include "Xaloc/Renderer/GraphicsContext.h"

#include "Core/VulkanDevice.h"
#include "Core/VulkanPhysicalDevice.h"
#include "Core/VulkanSwapChain.h"

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

		inline const Ref<VulkanDevice>& GetDevice() const { return m_Device; }
		inline const Ref<VulkanPhysicalDevice>& GetPhysicalDevice() const { return m_PhysicalDevice; }
		inline const VkSurfaceKHR& GetSurface() const { return m_Surface; }


		const const std::vector<VkCommandBuffer>& GetDrawCommandBuffers() const { return m_CommandBuffers; }	


	private:

		void CleanUp();
		
		void CreateInstance();
		void CreateSurface();
		void SetupDebugMessenger();

		
		// Temp
		void CreateRenderPass();
		
		void CreateDescriptorSetLayout();
		void CreateGraphicsPipeline();

		

		// UTIL FUNCTIONS
		
		bool CheckValidationLayerSupport() const;
		std::vector<const char*> GetRequiredExtensions() const;

		
	private:

		// How many frames should be processed concurrently.
		const int MAX_FRAMES_IN_FLIGHT = 2;


	private:
		
		GLFWwindow* m_Window;
		VkInstance m_Instance;

		VkSurfaceKHR m_Surface;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanPhysicalDevice> m_PhysicalDevice;
		
		size_t m_CurrentFrame = 0;


		// Swap chain

		Ref<VulkanSwapChain> m_SwapChain;




		// Temp
		
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;
		VkRenderPass m_RenderPass;
		VkPipeline m_GraphicsPipeline;
		
		std::vector<VkCommandBuffer> m_CommandBuffers;


		

		
		// Validation and debugging
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}