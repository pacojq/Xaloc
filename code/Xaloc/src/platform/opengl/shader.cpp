#include "xapch.h"
#include "platform/opengl/shader.h"
#include "xaloc/string.h"
#include "xaloc/debugging/assert.h"
#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Xaloc
{

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
	{
		return GL_VERTEX_SHADER;
	}
	if (type == "fragment" || type == "pixel")
	{
		return GL_FRAGMENT_SHADER;
	}
	XA_CORE_ASSERT(false, "Unknown shader type!");
	return 0;
}

static std::string ReadFile(const char* sFilepath)
{
	std::string result;
	std::ifstream in(sFilepath, std::ios::in | std::ios::binary);
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
		XA_CORE_ERROR("Could not open file: '{0}'", sFilepath);
	}

	return result;
}

static std::unordered_map<GLenum, std::string> PreProcess(
	const std::string sSource)
{
	// TODO: this is some awful parsing

	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = sSource.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = sSource.find_first_of("\r\n", pos);
		XA_CORE_ASSERT(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = sSource.substr(begin, eol - begin);
		XA_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

		size_t nextLinePos = sSource.find_first_not_of("\r\n", eol);
		pos = sSource.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = sSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? sSource.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

// Compile the shader.
// Code copied from https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
static uint32 Compile(
	const std::unordered_map<GLenum, std::string>& sources)
{
	GLuint program = glCreateProgram();
	XA_CORE_ASSERT(sources.size() <= 2, "A maximum of two shaders is supported.");

	int glShaderIDIndex = 0;
	std::array<GLenum, 2> glShaderIDs;

	for (auto& kv : sources)
	{
		GLenum type = kv.first;
		const GLchar* sSource = (const GLchar*)kv.second.c_str();

		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &sSource, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			XA_CORE_ERROR("{0}", infoLog.data());
			XA_CORE_ASSERT(false, "Shader compilation failure!");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs[glShaderIDIndex++] = shader;
	}

	// Link our program
	glLinkProgram(program);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(program);
		// Don't leak shaders either.
		for (auto id : glShaderIDs)
		{
			glDeleteShader(id);
		}

		XA_CORE_ERROR("{0}", infoLog.data());
		XA_CORE_ASSERT(false, "Shader link failure!");
		return 0;
	}

	// Always detach shaders after a successful link.
	for (auto id : glShaderIDs)
	{
		glDetachShader(program, id);
		glDeleteShader(id);
	}

	return program;
}

static void ExtractNameFromPath(
	int32& rOffset,
	int32& rCount,
	const char* sFilepath)
{
	// Extract name from filepath:
	// assets/shaders/Texture.glsl -> Texture.glsl -> Texture

	const int32 nLength = strlen(sFilepath);

	rOffset = StringFindLast(sFilepath, '/');
	if (rOffset < 0)
	{
		rOffset = StringFindLast(sFilepath, '\\');
	}
	if (rOffset < 0)
	{
		return;
	}
	rOffset++; // advance character

	const int32 nDot = StringFindLast(sFilepath, '.');
	rCount = (nDot < 0)
		? nLength - rOffset
		: nDot - rOffset;
}

ZShaderOpenGL::ZShaderOpenGL(const char* sFilepath)
{
	std::string source = ReadFile(sFilepath);
	auto shaderSources = PreProcess(source);
	m_RendererId = Compile(shaderSources);

	int32 nNameOffset;
	int32 nNameLength;
	ExtractNameFromPath(nNameOffset, nNameLength, sFilepath);
	XA_CORE_ASSERT(nNameOffset >= 0 && nNameLength > 0);
	
	m_sName = new char[nNameLength];
	memcpy(m_sName, sFilepath + nNameOffset, sizeof(char) * nNameLength);
}

ZShaderOpenGL::ZShaderOpenGL(
	const char* sName,
	const char* sSourceVertex,
	const char* sSourceFragment)
{
	strcpy(m_sName, sName);

	std::unordered_map<GLenum, std::string> shaderSources;
	shaderSources[GL_VERTEX_SHADER] = sSourceVertex;
	shaderSources[GL_FRAGMENT_SHADER] = sSourceFragment;
	m_RendererId = Compile(shaderSources);
}

ZShaderOpenGL::~ZShaderOpenGL()
{
	delete[] m_sName;
}

void ZShaderOpenGL::Bind() const
{
	glUseProgram(m_RendererId);
}

void ZShaderOpenGL::Unbind() const
{
	glUseProgram(0);
}

void ZShaderOpenGL::SetInt(
	const char* sName,
	int32 value)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniform1i(location, value);
}

void ZShaderOpenGL::SetIntArray(
	const char* sName,
	int32* aValues,
	uint32 nCount)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniform1iv(location, nCount, aValues);
}

void ZShaderOpenGL::SetFloat(
	const char* sName,
	float32 value)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniform1f(location, value);
}

void ZShaderOpenGL::SetFloat3(
	const char* sName,
	const glm::vec3& value)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniform3f(location, value.x, value.y, value.z);
}

void ZShaderOpenGL::SetFloat4(
	const char* sName,
	const glm::vec4& value)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void ZShaderOpenGL::SetMat4(
	const char* sName,
	const glm::mat4& value)
{
	GLint location = glGetUniformLocation(m_RendererId, sName);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

}