#pragma once
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/assets/Asset.h"


namespace Xaloc
{

class ZTexture : public Asset
{
public:
	virtual ~ZTexture() = default;

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	virtual uint32 GetRendererID() const = 0;

	// Give a pointer to a memory address and upload
	// stuff to the GPU.
	virtual void SetData(void* pData, uint32 nSize) = 0;

	virtual void Bind(uint32 nSlot = 0) const = 0;

	virtual bool operator==(const ZTexture& other) const = 0;
};

class ZTexture2D : public ZTexture
{
public:
	static ZRef<ZTexture2D> Create(uint32 nWidth, uint32 nHeight);
	static ZRef<ZTexture2D> Create(const char* sPath);

	static EAssetType GetStaticAssetType() { return EAssetType::Texture2D; }
	virtual EAssetType GetAssetType() const override { return GetStaticAssetType(); }
};

}