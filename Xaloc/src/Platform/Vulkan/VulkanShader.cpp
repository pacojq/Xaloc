#include "xapch.h"
#include "VulkanShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>


#include "VulkanContext.h"
#include "VulkanShared.h"
#include "VulkanUtils.h"
#include "Xaloc/Core/Application.h"

namespace Xaloc {


	VulkanShader::VulkanShader(const std::string& filepath)
	{
		// TODO not supported
		//XA_ASSERT(false, "One-file Vulkan Shader constructor not supported!");
	}


	VulkanShader::VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		// TODO

		auto vertShaderCode = ReadFile(vertexSrc);
		auto fragShaderCode = ReadFile(fragmentSrc);

		m_VertShaderModule = CreateShaderModule(vertShaderCode);
		m_FragShaderModule = CreateShaderModule(fragShaderCode);
	}


	VulkanShader::~VulkanShader()
	{
	}



	/*std::vector<char> VulkanShader::ReadFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}*/

	std::string VulkanShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			XA_CORE_ERROR("Could not open file: '{0}'", filepath);
		}

		return result;
	}

	

	

	VkShaderModule VulkanShader::CreateShaderModule(const std::string& code)
	{
		auto device = VulkanShared::Resources().Device->GetDevice();
		
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule), "Failed to create shader module!");

		return shaderModule;
	}


	



	void VulkanShader::Bind() const
	{
		// TODO
	}

	void VulkanShader::Unbind() const
	{
		// TODO
	}


	void VulkanShader::SetInt(const std::string& name, int value)
	{
		// TODO
	}

	void VulkanShader::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
		// TODO
	}

	void VulkanShader::SetFloat(const std::string& name, float value)
	{
		// TODO
	}

	void VulkanShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		// TODO
	}

	void VulkanShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		// TODO
	}

	void VulkanShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		// TODO
	}


}
