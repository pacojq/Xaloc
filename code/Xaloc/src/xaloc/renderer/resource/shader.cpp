#include "xapch.h"
#include "xaloc/core/hashing.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/renderer/resource/Shader.h"
#include "xaloc/renderer/renderer.h"
#include "platform/opengl/shader.h"
#include <string>


namespace Xaloc
{

ZRef<ZShader> ZShader::Create(const char* sFilepath)
{
	switch (RendererGetAPI())
	{
		case ERenderAPI::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case ERenderAPI::OpenGL:
			return std::make_shared<ZShaderOpenGL>(sFilepath);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

ZRef<ZShader> ZShader::Create(
	const char* sName,
	const char* sSourceVertex,
	const char* sSourceFragment)
{
	switch (RendererGetAPI())
	{
		case ERenderAPI::None:
			XA_CORE_ASSERT(false, "RendererAPI::None is curently not supported!");
			return nullptr;

		case ERenderAPI::OpenGL:
			return std::make_shared<ZShaderOpenGL>(
				sName,
				sSourceVertex,
				sSourceFragment);
	}

	XA_CORE_ASSERT(false, "Unknown RendererAPI!");
	return nullptr;
}

void ZShaderLibrary::Add(
	const char* sName,
	const ZRef<ZShader>& shader)
{
	const uint32 nName = HashFromString(sName);
	XA_CORE_ASSERT(
		m_Shaders.find(nName) == m_Shaders.end(),
		"Shader already exists!");
	m_Shaders[nName] = shader;
}

void ZShaderLibrary::Add(
	const ZRef<ZShader>& shader)
{
	const std::string sName = shader->GetName();
	Add(sName.c_str(), shader);
}

ZRef<ZShader> ZShaderLibrary::Load(
	const char* sFilepath)
{
	auto shader = ZShader::Create(sFilepath);
	Add(shader);
	return shader;
}

ZRef<ZShader> ZShaderLibrary::Load(
	const char* sName,
	const char* sFilepath)
{
	ZRef<ZShader> shader = ZShader::Create(sFilepath);
	Add(sName, shader);
	return shader;
}

ZRef<ZShader> ZShaderLibrary::Get(
	const char* sName) const
{
	const uint32 nName = HashFromString(sName);
	XA_CORE_ASSERT(
		m_Shaders.find(nName) != m_Shaders.end(),
		"Shader not found!");
	return m_Shaders.at(nName);
}

bool ZShaderLibrary::Exists(const char* sName) const
{
	const uint32 nName = HashFromString(sName);
	return m_Shaders.find(nName) != m_Shaders.end();
}

}