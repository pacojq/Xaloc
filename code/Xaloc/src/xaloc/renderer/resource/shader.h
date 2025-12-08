#pragma once
#include "xaloc/types.h"
#include <glm/glm.hpp>
#include <unordered_map>


namespace Xaloc
{

class ZShader
{
public:
	virtual ~ZShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetInt(
		const char* sName,
		int32 value) = 0;

	virtual void SetIntArray(
		const char* sName,
		int32* aValues,
		uint32 nCount) = 0;
	
	virtual void SetFloat(
		const char* sName,
		float32 value) = 0;

	virtual void SetFloat3(
		const char* sName,
		const glm::vec3& value) = 0;

	virtual void SetFloat4(
		const char* sName,
		const glm::vec4& value) = 0;

	virtual void SetMat4(
		const char* sName,
		const glm::mat4& value) = 0;

	virtual const char* GetName() const = 0;

	static ZRef<ZShader> Create(
		const char* sFilepath);

	static ZRef<ZShader> Create(
		const char* sName,
		const char* sSourceVertex,
		const char* sSourceFragment);
};

class ZShaderLibrary
{
public:
	void Add(
		const ZRef<ZShader>& shader);

	void Add(
		const char* sName,
		const ZRef<ZShader>& shader);

	// Takes the shader name from the file name
	ZRef<ZShader> Load(
		const char* sFilepath);

	ZRef<ZShader> Load(
		const char* sName,
		const char* sFilepath);

	ZRef<ZShader> Get(
		const char* sName) const;

	bool Exists(
		const char* sName) const;

private:
	std::unordered_map<uint32, ZRef<ZShader>> m_Shaders;
};

}