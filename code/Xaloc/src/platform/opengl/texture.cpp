#include "xapch.h"
#include "platform/opengl/texture.h"
#include "xaloc/debugging/assert.h"
#include "stb_image.h"
#include <glad/glad.h>


namespace Xaloc
{

static void GetFormats(
	GLenum& rInternalFormat,
	GLenum& rDataFormat,
	uint32 nChannelCount)
{
	rInternalFormat = 0;
	rDataFormat = 0;

	if (nChannelCount == 4)
	{
		rInternalFormat = GL_RGBA8;
		rDataFormat = GL_RGBA;
		return;
	}
	if (nChannelCount == 3)
	{
		rInternalFormat = GL_RGB8;
		rDataFormat = GL_RGB;
		return;
	}
	XA_CORE_ASSERT(false, "Invalid channel count.");
}

ZTexture2DOpenGL::ZTexture2DOpenGL(uint32 nWidth, uint32 nHeight)
	: m_nWidth(nWidth)
	, m_nHeight(nHeight)
{
	m_eInternalFormat = GL_RGBA8;
	m_eDataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_idRenderer);
	glTextureStorage2D(m_idRenderer, 1, m_eInternalFormat, m_nWidth, m_nHeight);

	// What kind of filters we use when the image gets scalled
	glTextureParameteri(m_idRenderer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_idRenderer, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_idRenderer, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_idRenderer, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

ZTexture2DOpenGL::ZTexture2DOpenGL(const char* sPath)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* pData =  stbi_load(sPath, &width, &height, &channels, 0);
	XA_CORE_ASSERT(pData, "Failed to load image!");

	m_nWidth = uint32(width);
	m_nHeight = uint32(height);

	GetFormats(m_eInternalFormat, m_eDataFormat, channels);
	XA_CORE_ASSERT(
		m_eInternalFormat & m_eDataFormat,
		"Format not supported!");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_idRenderer);
	glTextureStorage2D(m_idRenderer, 1, m_eInternalFormat, m_nWidth, m_nHeight);

	// What kind of filters we use when the image gets scalled
	glTextureParameteri(m_idRenderer, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_idRenderer, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_idRenderer, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_idRenderer, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_idRenderer, 0, 0, 0, m_nWidth, m_nHeight, m_eDataFormat, GL_UNSIGNED_BYTE, pData);

	stbi_image_free(pData);
}

ZTexture2DOpenGL::~ZTexture2DOpenGL()
{
	glDeleteTextures(1, &m_idRenderer);
}

void ZTexture2DOpenGL::SetData(void* pData, uint32 nSize)
{
	const uint32 nBytes = m_eDataFormat == GL_RGBA ? 4 : 3;
	const uint32 nExpectedSize = m_nWidth * m_nHeight * nBytes;

	XA_CORE_ASSERT(nSize == nExpectedSize, "Data must be the size of the entire texture!");
	glTextureSubImage2D(m_idRenderer, 0, 0, 0, m_nWidth, m_nHeight, m_eDataFormat, GL_UNSIGNED_BYTE, pData);
}

void ZTexture2DOpenGL::Bind(uint32 nSlot) const
{
	glBindTextureUnit(nSlot, m_idRenderer);
}

}