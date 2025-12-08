#pragma once
#include "xaloc/renderer/resource/texture.h"
#include <glad/glad.h>


namespace Xaloc
{

class ZTexture2DOpenGL : public ZTexture2D
{
public:
	ZTexture2DOpenGL(uint32 nWidth, uint32 nHeight);
	ZTexture2DOpenGL(const char* sPath);
	virtual ~ZTexture2DOpenGL();

	virtual uint32 GetWidth() const override { return m_nWidth; }
	virtual uint32 GetHeight() const override { return m_nHeight; }

	virtual uint32 GetRendererID() const override
	{
		return m_idRenderer;
	}

	virtual void SetData(void* pData, uint32 nSize) override;

	virtual void Bind(uint32 nSlot = 0) const override;

	virtual bool operator==(const ZTexture& other) const override
	{
		return m_idRenderer == ((ZTexture2DOpenGL&)other).m_idRenderer;
	}

private:
	uint32 m_nWidth;
	uint32 m_nHeight;
	uint32 m_idRenderer;
	GLenum m_eInternalFormat;
	GLenum m_eDataFormat;
};

}