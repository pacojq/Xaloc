#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"
#include "SubTexture2D.h"

namespace Xaloc {

	/*
		Xaloc's 2D renderer.
	*/
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		/// <summary>
		/// Draws the current batch and resets its state.
		/// </summary>
		static void Flush();



		

		// Primitives


		/// <summary>
		/// Draw a flat-colored quad
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="color"></param>
		/// <param name="depth"></param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float depth = 0.0f);
		
		/// <summary>
		/// Draw a flat-colored quad
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="color"></param>
		/// <param name="depth"></param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

		/// <summary>
		/// Draw a textured quad
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="texture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured quad
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="texture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured quad using a SubTexture2D
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="subTexture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured quad using a SubTexture2D
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="subTexture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured quad using a SubTexture2D
		/// </summary>
		/// <param name="transform"></param>
		/// <param name="subTexture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		




		

		/// <summary>
		/// Draw a flat-colored, rotated quad.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="color"></param>
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		/// <summary>
		/// Draw a flat-colored, rotated quad.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="color"></param>
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		/// <summary>
		/// Draw a textured, rotated quad.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="texture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured, rotated quad.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="texture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));

		/// <summary>
		/// Draw a textured, rotated quad using a SubTexture2D.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="subTexture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));
		
		/// <summary>
		/// Draw a textured, rotated quad using a SubTexture2D.
		/// </summary>
		/// <param name="position"></param>
		/// <param name="size"></param>
		/// <param name="rotation">Rotation, in radians</param>
		/// <param name="subTexture"></param>
		/// <param name="tilingFactor"></param>
		/// <param name="tintColor"></param>
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f, const glm::vec4 tintColor = glm::vec4(1.0f));




		static void DrawLine(const glm::vec2& p0, const glm::vec2& p1, const glm::vec4& color = glm::vec4(1.0f), float width = 1.0f);



		
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