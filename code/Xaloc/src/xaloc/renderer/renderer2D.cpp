#include "xapch.h"
#include "xaloc/types.h"
#include "xaloc/renderer/renderer.h"
#include "xaloc/renderer/camera.h"
#include "xaloc/renderer/renderer2D.h"
#include "xaloc/renderer/cameras/editor.h"
#include "xaloc/renderer/geometry/quad.h"
#include "xaloc/renderer/geometry/index.h"
#include "xaloc/renderer/geometry/vertex.h"
#include "xaloc/renderer/pipeline/pipeline.h"
#include "xaloc/renderer/resource/shader.h"
#include <glm/gtc/matrix_transform.hpp>


namespace Xaloc
{

static const uint32 C_MAX_QUADS = 256; // Max per draw call
static const uint32 C_MAX_VERTICES = C_MAX_QUADS * 4;
static const uint32 C_MAX_INDICES = C_MAX_QUADS * 6;
static const uint32 C_MAX_TEXTURE_SLOTS = 32; // TODO: render capabilities API

struct SBlitVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoord;
};

// Global 2D renderer data
struct SRendererData
{
	ZRef<ZRenderPipeline> QuadPipeline;
	ZRef<ZIndexBuffer> QuadIndexBuffer;
	ZRef<ZVertexBuffer> QuadVertexBuffer;

	ZRef<ZShader> TextureShader;
	ZRef<ZTexture2D> WhiteTexture;

	ZRef<ZRenderPipeline> BlitPipeline;
	ZRef<ZIndexBuffer> BlitIndexBuffer;
	ZRef<ZVertexBuffer> BlitVertexBuffer;

	ZDebugRenderer2D* pDebug = nullptr;
};

struct SBlitData
{
	SBlitVertex aBlitVertexBuffer[4]; // fullscreen blit
};

// Data specific for the current batch
struct SBatchData
{
	uint32 QuadIndexCount = 0; // How many indices are we going to draw

	SQuadVertex* pQuadVertexBuffer = nullptr;
	SQuadVertex aQuadVertexBuffer[C_MAX_VERTICES];

	std::array<ZRef<ZTexture2D>, C_MAX_TEXTURE_SLOTS> TextureSlots; // TODO change to asset handle
	uint32 TextureSlotIndex = 1; // slot 0 = white texture
};

static bool s_bInit = false;
static SRendererData s_GlobalData;
static SBatchData s_BatchData;
static SBlitData s_BlitData;
static ZQuadBuilder s_QuadBuilder;

static float32 FindTextureIndex(const ZRef<ZTexture2D> pTexture)
{
	float32 fTextureIndex = 0.0f; // Default to white texture

	// Start the loop at 1, since 0 is the default white texture
	for (uint32_t i = 1; i < s_BatchData.TextureSlotIndex; i++)
	{
		if (*s_BatchData.TextureSlots[i].get() == *pTexture.get()) // TODO asset handles
		{
			fTextureIndex = (float32)i;
			break;
		}
	}
	return fTextureIndex;
}

static void BatchStart()
{
	// Reset pointer to the first element of the Vertex Buffer
	s_BatchData.QuadIndexCount = 0;
	s_BatchData.pQuadVertexBuffer = s_BatchData.aQuadVertexBuffer;

	s_BatchData.TextureSlotIndex = 1; // slot 0 = white texture
}

static void BatchPushQuads(const ZQuadBuilder& quads)
{
	if (s_BatchData.QuadIndexCount >= C_MAX_INDICES)
	{
		ZRenderer2D::Flush();
		BatchStart();
	}

	for (uint32 nQuad = 0; nQuad < 4; ++nQuad)
	{
		*s_BatchData.pQuadVertexBuffer = quads.Get(nQuad);
		s_BatchData.pQuadVertexBuffer++;
	}
	s_BatchData.QuadIndexCount += 6;

#if RENDER_STATS_ENABLED
	SStatsRenderer2D& rStats = s_GlobalData.pDebug->WriteStats();
	rStats.nQuadCount++;
#endif
}

static float32 BatchBindTexture(const ZRef<ZTexture2D>& pTexture)
{
	float32 fTextureIndex = FindTextureIndex(pTexture);
	if (fTextureIndex == 0.0f)
	{
		if (s_BatchData.TextureSlotIndex >= C_MAX_TEXTURE_SLOTS)
		{
			ZRenderer2D::Flush();
			BatchStart();
		}

		fTextureIndex = (float32)s_BatchData.TextureSlotIndex;
		s_BatchData.TextureSlots[s_BatchData.TextureSlotIndex] = pTexture;
		s_BatchData.TextureSlotIndex++;
	}
	return fTextureIndex;
}

void ZRenderer2D::Init()
{
	XA_CORE_ASSERT(!s_bInit, "Renderer2D is already initialized!");
	s_bInit = true;

	s_GlobalData.pDebug = new ZDebugRenderer2D();

	// CREATE QUADS PIPELINE
	{
		SRenderPipelineDefinition pipelineSpecification;
		pipelineSpecification.Layout = {
			{ EVertexDataType::Float3, "a_Position" },
			{ EVertexDataType::Float4, "a_Color" },
			{ EVertexDataType::Float2, "a_TexCoord" },
			{ EVertexDataType::Float,  "a_TexIndex" },
			{ EVertexDataType::Float,  "a_TilingFactor" }
		};
		s_GlobalData.QuadPipeline = ZRenderPipeline::Create(pipelineSpecification);

		s_GlobalData.QuadVertexBuffer = ZVertexBuffer::Create(C_MAX_VERTICES * sizeof(SQuadVertex));

		// Create index buffer
		uint32 aQuadIndices[C_MAX_INDICES];

		uint32 nOffset = 0;
		for (uint32 nIndex = 0; nIndex < C_MAX_INDICES; nIndex += 6)
		{
			aQuadIndices[nIndex + 0] = nOffset + 0;
			aQuadIndices[nIndex + 1] = nOffset + 1;
			aQuadIndices[nIndex + 2] = nOffset + 2;

			aQuadIndices[nIndex + 3] = nOffset + 2;
			aQuadIndices[nIndex + 4] = nOffset + 3;
			aQuadIndices[nIndex + 5] = nOffset + 0;

			nOffset += 4;
		}

		s_GlobalData.QuadIndexBuffer = ZIndexBuffer::Create(aQuadIndices, C_MAX_INDICES);
	}

	// CREATE BLIT PIPELINE
	{
		SRenderPipelineDefinition pipelineDefinition;
		pipelineDefinition.Layout = {
			{ EVertexDataType::Float3, "a_Position" },
			{ EVertexDataType::Float2, "a_TexCoord" }
		};
		s_GlobalData.BlitPipeline = ZRenderPipeline::Create(pipelineDefinition);

		// fullscreen blit takes 4 vertices

		uint32 aQuadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		s_BlitData.aBlitVertexBuffer[0].Position = { -1.0f, -1.0f, 0.0f };
		s_BlitData.aBlitVertexBuffer[0].TexCoord = { 0.0f, 1.0f };
		s_BlitData.aBlitVertexBuffer[1].Position = { 1.0f, -1.0f, 0.0f };
		s_BlitData.aBlitVertexBuffer[1].TexCoord = { 1.0f, 1.0f };
		s_BlitData.aBlitVertexBuffer[2].Position = { 1.0f, 1.0f, 0.0f };
		s_BlitData.aBlitVertexBuffer[2].TexCoord = { 1.0f, 0.0f };
		s_BlitData.aBlitVertexBuffer[3].Position = { -1.0f, 1.0f, 0.0f };
		s_BlitData.aBlitVertexBuffer[3].TexCoord = { 0.0f, 0.0f };

		s_GlobalData.BlitVertexBuffer = ZVertexBuffer::Create(4 * sizeof(SBlitVertex));
		s_GlobalData.BlitIndexBuffer = ZIndexBuffer::Create(aQuadIndices, 6);
	}

	// INITIALIZE TEXTURE DATA
	{
		s_GlobalData.WhiteTexture = ZTexture2D::Create(1, 1);
		int32 whiteTextureData = 0xffffffff;
		s_GlobalData.WhiteTexture->SetData(&whiteTextureData, sizeof(int32));

		int32 aSamplers[C_MAX_TEXTURE_SLOTS];
		for (int32 nSampler = 0; nSampler < C_MAX_TEXTURE_SLOTS; ++nSampler)
		{
			aSamplers[nSampler] = nSampler;
		}
		s_GlobalData.TextureShader = ZShader::Create("assets/shaders/Texture.glsl");
		s_GlobalData.TextureShader->Bind();
		s_GlobalData.TextureShader->SetIntArray("u_Textures", aSamplers, C_MAX_TEXTURE_SLOTS);

		// Slot 0 is for white texture
		s_BatchData.TextureSlots[0] = s_GlobalData.WhiteTexture;
	}
}

void ZRenderer2D::Shutdown()
{
	delete s_GlobalData.pDebug;
}

ZDebugRenderer2D* ZRenderer2D::GetDebug()
{
#if RENDER_STATS_ENABLED
	return s_GlobalData.pDebug;
#else
	return nullptr;
#endif
}

void ZRenderer2D::BeginScene(glm::mat4 viewProj)
{
	// TODO: assert batch is not started

	s_GlobalData.TextureShader->Bind();
	s_GlobalData.TextureShader->SetMat4("u_ViewProjection", viewProj);
	BatchStart();
}

void ZRenderer2D::BeginScene(const ZCamera& camera, const glm::mat4& transform)
{
	//                   Projection matrix      * View matrix
	glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);
	BeginScene(viewProj);
}

void ZRenderer2D::BeginScene(const ZEditorCamera& camera)
{
	BeginScene(camera.GetViewProjection());
}

void ZRenderer2D::EndScene()
{
	// Flush everything
	Flush();
}

void ZRenderer2D::Blit(const ZRef<ZShader>& shader)
{
	shader->Bind();

	s_GlobalData.BlitVertexBuffer->SetData(
		s_BlitData.aBlitVertexBuffer, sizeof(SBlitVertex) * 4);

	s_GlobalData.BlitPipeline->Bind();
	// TODO bind vertex buffer?
	s_GlobalData.BlitIndexBuffer->Bind();

	RendererGet()->DrawBlit();
}

void ZRenderer2D::Flush()
{
	if (s_BatchData.QuadIndexCount == 0)
	{
		return; // Nothing to draw
	}

	// Upload data to GPU
	const uint32 nDataSize = (uint32)((uint8*)s_BatchData.pQuadVertexBuffer - (uint8*)s_BatchData.aQuadVertexBuffer); // Count how many elements we have by looking at the difference between pointers	
	s_GlobalData.QuadVertexBuffer->SetData(s_BatchData.aQuadVertexBuffer, nDataSize);

	// Bind textures
	for (uint32_t i = 0; i < s_BatchData.TextureSlotIndex; i++)
	{
		s_BatchData.TextureSlots[i]->Bind(i);
	}

	// Draw everything
	s_GlobalData.QuadPipeline->Bind();
	// TODO bind vertex buffer?
	s_GlobalData.QuadIndexBuffer->Bind();
	RendererGet()->DrawIndexed(s_BatchData.QuadIndexCount);

#if RENDER_STATS_ENABLED
	SStatsRenderer2D& rStats = s_GlobalData.pDebug->WriteStats();
	rStats.nDrawCalls++;
#endif
}

void ZRenderer2D::DrawQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	const glm::vec4& color,
	float32 depth)
{
	DrawQuad({ position.x, position.y, depth }, size, color);
}

void ZRenderer2D::DrawQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	const glm::vec4& color)
{
	const glm::mat4 transform = 
		glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(color);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	const ZRef<ZTexture2D>& texture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	DrawQuad(
		{ position.x, position.y, 0.0f },
		size,
		texture,
		tilingFactor,
		tintColor);
}

void ZRenderer2D::DrawQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	const ZRef<ZTexture2D>& texture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	const float32 fTextureIndex = BatchBindTexture(texture);
	const glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(tintColor);
	s_QuadBuilder.SetTextureIndex(fTextureIndex);
	s_QuadBuilder.SetTilingFactor(tilingFactor);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	const ZRef<ZSubTexture2D>& subTexture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	const glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.0f }) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, subTexture, tilingFactor, tintColor);
}

void ZRenderer2D::DrawQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	const ZRef<ZSubTexture2D>& subTexture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	const glm::mat4 transform =
		glm::translate(glm::mat4(1.0f), position) *
		glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

	DrawQuad(transform, subTexture, tilingFactor, tintColor);
}

void ZRenderer2D::DrawQuad(
	const glm::mat4& transform,
	const ZRef<ZSubTexture2D>& subTexture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	// TODO change DrawRotatedQuad stuff to use this method

	const ZRef<ZTexture2D> pTexture = subTexture->GetTexture();
	const glm::vec2* aTextureCoords = subTexture->GetTexCoords();
	const float32 fTextureIndex = BatchBindTexture(pTexture);

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(tintColor);
	s_QuadBuilder.SetTexcoords(aTextureCoords);
	s_QuadBuilder.SetTextureIndex(fTextureIndex);
	s_QuadBuilder.SetTilingFactor(tilingFactor);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	float32 rotation,
	const glm::vec4& color)
{
	DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	float32 rotation,
	const glm::vec4& color)
{
	const glm::mat4 mIdentity = glm::mat4(1.0f);
	const glm::mat4 transform =
		glm::translate(mIdentity, position) *
		glm::rotate(mIdentity, rotation, { 0.0f, 0.0f, 1.0f }) *
		glm::scale(mIdentity, { size.x, size.y, 1.0f });

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(color);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	float32 rotation,
	const ZRef<ZTexture2D>& texture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	DrawRotatedQuad(
		{ position.x, position.y, 0.0f },
		size,
		rotation,
		texture,
		tilingFactor,
		tintColor);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	float32 rotation,
	const ZRef<ZTexture2D>& texture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	const float32 fTextureIndex = BatchBindTexture(texture);
	const glm::mat4 mIdentity = glm::mat4(1.0f);
	const glm::mat4 transform =
		glm::translate(mIdentity, position) *
		glm::rotate(mIdentity, rotation, { 0.0f, 0.0f, 1.0f }) *
		glm::scale(mIdentity, { size.x, size.y, 1.0f });

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(tintColor);
	s_QuadBuilder.SetTextureIndex(fTextureIndex);
	s_QuadBuilder.SetTilingFactor(tilingFactor);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec2& position,
	const glm::vec2& size,
	float32 rotation,
	const ZRef<ZSubTexture2D>& subTexture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	DrawRotatedQuad(
		{ position.x, position.y, 0.0f },
		size,
		rotation,
		subTexture,
		tilingFactor,
		tintColor);
}

void ZRenderer2D::DrawRotatedQuad(
	const glm::vec3& position,
	const glm::vec2& size,
	float32 rotation,
	const ZRef<ZSubTexture2D>& subTexture,
	float32 tilingFactor,
	const glm::vec4 tintColor)
{
	const ZRef<ZTexture2D> pTexture = subTexture->GetTexture();
	const glm::vec2* aTextureCoords = subTexture->GetTexCoords();
	const float32 fTextureIndex = BatchBindTexture(pTexture);

	const glm::mat4 mIdentity = glm::mat4(1.0f);
	const glm::mat4 transform =
		glm::translate(mIdentity, position) *
		glm::rotate(mIdentity, rotation, { 0.0f, 0.0f, 1.0f }) *
		glm::scale(mIdentity, { size.x, size.y, 1.0f });

	s_QuadBuilder.Reset();
	s_QuadBuilder.SetTransform(transform);
	s_QuadBuilder.SetColor(tintColor);
	s_QuadBuilder.SetTexcoords(aTextureCoords);
	s_QuadBuilder.SetTextureIndex(fTextureIndex);
	s_QuadBuilder.SetTilingFactor(tilingFactor);
	BatchPushQuads(s_QuadBuilder);
}

void ZRenderer2D::DrawLine(
	const glm::vec2& p0,
	const glm::vec2& p1,
	const glm::vec4& color,
	float32 width)
{
	const glm::vec3 middle = { (p0 + p1) / 2.0f, 1.0f };
	const glm::vec2 delta = p1 - p0;

	const float32 dist = glm::sqrt(delta.x * delta.x + delta.y * delta.y);
	const float32 rot = glm::atan(delta.y, delta.x);

	const glm::vec2 size = { dist, width / C_PIXELS_PER_UNIT };

	DrawRotatedQuad(middle, size, rot, color);
}

}