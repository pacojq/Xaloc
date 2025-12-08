#pragma once
#include "xaloc/renderer/debugging/renderer2D.h"
#include "xaloc/renderer/resource/texture.h"
#include "xaloc/renderer/resource/subtexture.h"
#include "xaloc/renderer/resource/Shader.h"

namespace Xaloc {
	class ZCamera;
	class ZEditorCamera;
}

namespace Xaloc
{

// TODO: define px per unit in project config
#define C_PIXELS_PER_UNIT 16.0f

/// <summary>
/// Xaloc's 2D renderer.
/// </summary>
class ZRenderer2D
{
public:
	static void Init();
	static void Shutdown();

	static ZDebugRenderer2D* GetDebug();

	/// <summary>
	/// Begins a scene with a given view projection matrix.
	/// </summary>
	/// <param name="viewProj"></param>
	static void BeginScene(
		glm::mat4 viewProj);

	/// Utility BeginScene
	static void BeginScene(
		const ZCamera& camera,
		const glm::mat4& transform);

	/// Utility BeginScene
	static void BeginScene(
		const ZEditorCamera& camera);

	static void EndScene();

	/// <summary>
	/// Draws the current batch and resets its state.
	/// </summary>
	static void Flush();

	static void Blit(
		const ZRef<ZShader>& shader);

	// Primitives

	/// <summary>
	/// Draw a flat-colored quad
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="color"></param>
	/// <param name="depth"></param>
	static void DrawQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		const glm::vec4& color,
		float32 depth = 0.0f);
		
	/// <summary>
	/// Draw a flat-colored quad
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="color"></param>
	/// <param name="depth"></param>
	static void DrawQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		const glm::vec4& color);

	/// <summary>
	/// Draw a textured quad
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="texture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		const ZRef<ZTexture2D>& texture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured quad
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="texture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		const ZRef<ZTexture2D>& texture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured quad using a SubTexture2D
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="subTexture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		const ZRef<ZSubTexture2D>& subTexture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured quad using a SubTexture2D
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="subTexture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		const ZRef<ZSubTexture2D>& subTexture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured quad using a SubTexture2D
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="subTexture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawQuad(
		const glm::mat4& transform,
		const ZRef<ZSubTexture2D>& subTexture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a flat-colored, rotated quad.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="color"></param>
	static void DrawRotatedQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		float32 rotation,
		const glm::vec4& color);

	/// <summary>
	/// Draw a flat-colored, rotated quad.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="color"></param>
	static void DrawRotatedQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		float32 rotation,
		const glm::vec4& color);

	/// <summary>
	/// Draw a textured, rotated quad.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="texture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawRotatedQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		float32 rotation,
		const ZRef<ZTexture2D>& texture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured, rotated quad.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="texture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawRotatedQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		float32 rotation,
		const ZRef<ZTexture2D>& texture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	/// <summary>
	/// Draw a textured, rotated quad using a SubTexture2D.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="subTexture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawRotatedQuad(
		const glm::vec2& position,
		const glm::vec2& size,
		float32 rotation,
		const ZRef<ZSubTexture2D>& subTexture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));
		
	/// <summary>
	/// Draw a textured, rotated quad using a SubTexture2D.
	/// </summary>
	/// <param name="position"></param>
	/// <param name="size"></param>
	/// <param name="rotation">Rotation, in radians</param>
	/// <param name="subTexture"></param>
	/// <param name="tilingFactor"></param>
	/// <param name="tintColor"></param>
	static void DrawRotatedQuad(
		const glm::vec3& position,
		const glm::vec2& size,
		float32 rotation,
		const ZRef<ZSubTexture2D>& subTexture,
		float32 tilingFactor = 1.0f,
		const glm::vec4 tintColor = glm::vec4(1.0f));

	static void DrawLine(
		const glm::vec2& p0,
		const glm::vec2& p1,
		const glm::vec4& color = glm::vec4(1.0f),
		float32 width = 1.0f);

};

}