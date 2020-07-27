#pragma once

#include "Xaloc/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan_core.h>

namespace Xaloc {

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::string& filepath);
		VulkanShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~VulkanShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		virtual const std::string GetName() const override { return m_Name; }


		// TODO const VkPipelineLayout& GetPipelineLayout() const { return m_PipelineLayout; }

		inline const VkShaderModule& GetVertShaderModule() const { return m_VertShaderModule; }
		inline const VkShaderModule& GetFragShaderModule() const { return m_FragShaderModule; }

	private:
		VkShaderModule VulkanShader::CreateShaderModule(const std::string& code);
		static std::string ReadFile(const std::string& filename);

	private:
		uint32_t m_RendererId;
		std::string m_Name;

		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};

}
