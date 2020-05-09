#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.h"

namespace Xaloc {

	/*
		The renderer API.

		Everything any renderer in Xaloc can do will go here, so
		it can be called by Render Commands.

		Implementation will go appart, living in the specific
		Platform folder. For example, if we're using OpenGL our
		Renderer API implementation will go inside:
			
			src/Platform/OpenGL/OpenGLRendererAPI.h
			src/Platform/OpenGL/OpenGLRendererAPI.cpp
	*/
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};

	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		inline static API GetAPI() { return s_API; }
		inline static void SetAPI(const API api) { s_API = api; }

	private:
		static API s_API;
	};

}