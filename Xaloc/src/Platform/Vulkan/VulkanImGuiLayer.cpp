#include "xapch.h"
#include "VulkanImGuiLayer.h"

#include "Xaloc/Core/Application.h"
#include "Xaloc/Renderer/Renderer.h"

#include "VulkanShared.h"
#include "VulkanContext.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Xaloc/ImGui/ImGuizmo.h"

#define IMGUI_IMPL_API
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_vulkan.h"

#include <GLFW/glfw3.h>


namespace Xaloc {


	static void check_vk_result(VkResult err)
	{
		if (err == 0)
			return;
		fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
		if (err < 0)
			abort();
	}

	
	VulkanImGuiLayer::VulkanImGuiLayer()
		: ImGuiLayer()
	{
	}

	
	void VulkanImGuiLayer::OnAttach()
	{
		SetupImGui();
		//VulkanImGuiLayer* instance = this;
		//Renderer::Submit([instance]()
		//	{
			Application& app = Application::Get();
			GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

			auto context = VulkanShared::Resources().Context;
			auto swapChain = VulkanShared::Resources().SwapChain;
			auto device = VulkanShared::Resources().Device;
			auto physicalDevice = VulkanShared::Resources().PhysicalDevice;

			VkDescriptorPool descriptorPool;

			// Create Descriptor Pool
			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};
			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
			pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;
			auto err = vkCreateDescriptorPool(device->GetDevice(), &pool_info, nullptr, &descriptorPool);
			check_vk_result(err);

			// Setup Platform/Renderer bindings
			ImGui_ImplGlfw_InitForVulkan(window, true);
			ImGui_ImplVulkan_InitInfo init_info = {};
			init_info.Instance = VulkanShared::Resources().Instance;
			init_info.PhysicalDevice = physicalDevice->GetPhysicalDevice();
			init_info.Device = device->GetDevice();
			init_info.QueueFamily = -1; // TODO graphics queue family index
			init_info.Queue = device->GetGraphicsQueue();
			init_info.PipelineCache = nullptr;
			init_info.DescriptorPool = descriptorPool;
			init_info.Allocator = nullptr;
			init_info.MinImageCount = 2;
			init_info.ImageCount = 2;// TODO VulkanShared::Resources().SwapChain->GetVulkanS; vulkanContext->GetSwapChain().GetImageCount();
			init_info.CheckVkResultFn = check_vk_result;
			// TODO ImGui_ImplVulkan_Init(&init_info, context->GetRenderPass());

			// Upload Fonts
			{
				// Use any command queue

				// TODO VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);
				// TODO ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
				// TODO device->FlushCommandBuffer(commandBuffer);

				err = vkDeviceWaitIdle(device->GetDevice());
				check_vk_result(err);
				ImGui_ImplVulkan_DestroyFontUploadObjects();
			}
		//});
	}

	void VulkanImGuiLayer::OnDetach()
	{
		auto device = VulkanShared::Resources().Device->GetDevice();

		auto err = vkDeviceWaitIdle(device);
		check_vk_result(err);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



	void VulkanImGuiLayer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		// ImGui::ShowDemoWindow();
	}

	void VulkanImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}





	void VulkanImGuiLayer::OnImGuiRender()
	{
		// TODO
	}


}
