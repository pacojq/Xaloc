#pragma once
#include "xaloc/renderer/resource/shader.h"
#include <glm/glm.hpp>


namespace Xaloc
{

class ZShaderOpenGL : public ZShader
{
public:
	ZShaderOpenGL(const char* filepath);
	ZShaderOpenGL(
		const char* name,
		const char* sSourceVertex,
		const char* sSourceFragment);
	virtual ~ZShaderOpenGL();

	virtual void Bind() const override;
	virtual void Unbind() const override;

	virtual void SetInt(
		const char* sName,
		int32 value) override;

	virtual void SetIntArray(
		const char* sName,
		int32* values,
		uint32 nCount) override;

	virtual void SetFloat(
		const char* sName,
		float32 value) override;

	virtual void SetFloat3(
		const char* sName,
		const glm::vec3& value) override;

	virtual void SetFloat4(
		const char* sName,
		const glm::vec4& value) override;

	virtual void SetMat4(
		const char* sName,
		const glm::mat4& value) override;

	virtual const char* GetName() const override { return m_sName; }

private:
	uint32_t m_RendererId;
	char* m_sName;
};

}