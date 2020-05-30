#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Xaloc {

	/*
		Arkhe's 2D renderer.
	*/
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();


		// Primitives



		// Draw a flat-colored quad
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float depth = 0.0f);
		
		// Draw a flat-colored quad
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		// Draw a textured quad
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		// Draw a textured quad
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		// Draw a textured quad using a SubTexture2D
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		// Draw a textured quad using a SubTexture2D
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		


		// Draw a flat-colored, rotated quad. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		// Draw a flat-colored, rotated quad. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		// Draw a textured, rotated quad. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		// Draw a textured, rotated quad. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		// Draw a textured, rotated quad using a SubTexture2D. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		
		// Draw a textured, rotated quad using a SubTexture2D. Expecting rotation in radians.
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));







		
		// Stats

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();






	private:
		static float FindTextureIndex(const Ref<Texture2D> texture);
		static void FlushAndReset();
	};

}