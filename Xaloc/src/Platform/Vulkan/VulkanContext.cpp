#include "xapch.h"
#include "VulkanContext.h"

#include "Xaloc/Core/Application.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VulkanContextUtils.h"

#include <map>
#include <set>
#include <vector>
#include <optional>


namespace Xaloc {

	
	VulkanContext::VulkanContext(GLFWwindow* windowHandle)
		: m_Window(windowHandle)
	{
		XA_CORE_ASSERT(windowHandle, "Window handle is null!");

#ifdef XA_DEBUG
		m_EnableValidationLayers = true;
#else
		m_EnableValidationLayers = true;
#endif
		
		// TODO
	}

	VulkanContext::~VulkanContext()
	{
		
	}

	

	void VulkanContext::Init()
	{
		XA_CORE_TRACE("Vulkan Context initializing...");

		
		CreateInstance();
		XA_CORE_TRACE("    Instance created");
		SetupDebugMessenger();

		// Create surface
		VK_CHECK_RESULT(glfwCreateWindowSurface(m_Instance, m_Window, nullptr, &m_Surface), "Failed to create surface!");
		XA_CORE_TRACE("    Surface ready");
		
		PickPhysicalDevice();
		XA_CORE_TRACE("    Physical device ready");

		CreateLogicalDevice();
		XA_CORE_TRACE("    Logical device ready");
		
		CreateSwapChain();
		XA_CORE_TRACE("    Swap chain ready");
		
		/* TODO

		CreateRenderPass();
		LOG("[RENDER PASS READY]");

		CreateDescriptorSetLayout();
		CreateGraphicsPipeline();
		LOG("[PIPELINE READY]");
		
		CreateCommandPool();
		LOG("[COMMAND POOL READY]");

		CreateDepthResources();
		LOG("[DEPTH RESOURCES READY]");

		CreateFrameBuffers();
		LOG("[FRAME BUFFERS READY]");
		 
		 */
		
		XA_CORE_TRACE("Vulkan Context ready!");
	}



	void VulkanContext::CleanUp()
	{
		vkDestroyDevice(m_Device, nullptr);

		if (m_EnableValidationLayers)
		{
			VulkanContextUtils::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyInstance(m_Instance, nullptr);
	}
		



	
	void VulkanContext::CreateInstance()
	{
		// Application info
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = Application::GetName().c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // TODO app version
		appInfo.pEngineName = "Xaloc Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(XALOC_VERSION_MAJOR, XALOC_VERSION_MINOR, XALOC_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_0; // TODO jump to 1.1

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


		// Validation layers
		XA_CORE_ASSERT(!(m_EnableValidationLayers && !CheckValidationLayerSupport()),
			"Validation layers requested, but not available!");

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}


		// Extensions
		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		XA_CORE_INFO("  Vulkan Extensions supported:   {0}", extensions.size());
		for (const auto& extension : extensions)
		{
			XA_CORE_INFO("    - {0}", extension);
		}


		// Create instance
		VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &m_Instance), "Failed to create instance!");
	}


	bool VulkanContext::CheckValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
				return false;
		}
		return true;
	}

	std::vector<const char*> VulkanContext::GetRequiredExtensions() const
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (m_EnableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}


	


	

	void VulkanContext::SetupDebugMessenger()
	{
		if (!m_EnableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = VulkanDebugCallback;
		createInfo.pUserData = nullptr; // Optional

		VK_CHECK_RESULT(VulkanContextUtils::CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger), "");
	}

	

	
	
	void VulkanContext::PickPhysicalDevice()
	{
		m_PhysicalDevice = VK_NULL_HANDLE;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr);

		XA_CORE_ASSERT(deviceCount > 0, "Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data());


		// Use an ordered map to automatically sort candidates by increasing score
		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : devices)
		{
			int score = VulkanContextUtils::RateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0)
		{
			m_PhysicalDevice = candidates.rbegin()->second;
		}

		XA_CORE_ASSERT(m_PhysicalDevice != VK_NULL_HANDLE, "Failed to find a suitable GPU!");
	}





	void VulkanContext::CreateLogicalDevice()
	{
		QueueFamilyIndices indices = VulkanContextUtils::FindQueueFamilies(m_PhysicalDevice, m_Surface);

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

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		VK_CHECK_RESULT(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create logical device!");
		vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
		vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &m_PresentQueue);
	}

	

	
	void VulkanContext::CreateSwapChain()
	{
		int width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		VulkanSwapChainSpecification spec;
		spec.Device = m_Device;
		spec.PhysicalDevice = m_PhysicalDevice;
		spec.Surface = m_Surface;
		spec.Width = width;
		spec.Height = height;
		
		m_SwapChain = CreateRef<VulkanSwapChain>(spec);
	}

	





	

	void VulkanContext::SwapBuffers()
	{
		// TODO
	}

}
