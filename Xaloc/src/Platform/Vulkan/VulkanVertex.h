#pragma once

// TODO THIS FILE IS TEMPORARY, SWITCH TO BUFFERLAYOUT

#include <vulkan/vulkan.h>

#include <glm/glm.hpp>
#include <array>

struct VulkanVertex {

	glm::vec3 Pos;
	glm::vec3 Color;
	glm::vec2 TexCoord;

	bool operator==(const VulkanVertex& other) const
	{
		return Pos == other.Pos && Color == other.Color && TexCoord == other.TexCoord;
	}

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(VulkanVertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(VulkanVertex, Pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(VulkanVertex, Color);

		// Texcoord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(VulkanVertex, TexCoord);

		return attributeDescriptions;
	}
};


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std {
	template<> struct hash<VulkanVertex> {
		size_t operator()(VulkanVertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.Pos) ^
				(hash<glm::vec3>()(vertex.Color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoord) << 1);
		}
	};
}