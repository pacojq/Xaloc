#pragma once
#include "xaloc/types.h"
#include <glm/glm.hpp>
#include <memory>


namespace Xaloc
{

enum class ERenderAPI
{
	None = 0,
	OpenGL = 1
};

/// <summary>
/// The renderer API.
///
/// Everything any renderer in Xaloc can do will go here, so
/// it can be called by Render Commands.
/// 
/// Implementation will go appart, living in the specific
/// Platform folder.For example, if we're using OpenGL our
/// Renderer API implementation will go inside :
/// 
///     src / Platform / OpenGL / OpenGLRendererAPI.h
///     src / Platform / OpenGL / OpenGLRendererAPI.cpp
/// 
/// </summary>
class ZRendererAPI
{
public:
	~ZRendererAPI() = default;

	virtual void Init() = 0;
	virtual void SetViewport(
		uint32 nX,
		uint32 nY,
		uint32 nWidth,
		uint32 nHeight) = 0;

	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;

	virtual void DrawIndexed(uint32 nIndexCount = 0) = 0;

	virtual void DrawBlit() = 0;
};

}