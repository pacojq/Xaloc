#pragma once
#include "xaloc/core/assets/asset.h"

namespace Xaloc {
	struct SAssetMetadata;
}


namespace Xaloc
{

class ZAssetSerializer
{
public:
	virtual void Serialize(
		const SAssetMetadata& metadata,
		const ZRef<Asset>& asset) const = 0;

	virtual bool TryLoadData(
		ZRef<Asset>& rAsset,
		const SAssetMetadata& metadata) const = 0;

	virtual bool GetInvalidAsset(
		ZRef<Asset>& rAsset) = 0;
};

class ZAssetSerializerTexture : public ZAssetSerializer
{
public:
	virtual void Serialize(
		const SAssetMetadata& metadata,
		const ZRef<Asset>& asset) const override
	{
	}

	virtual bool TryLoadData(
		ZRef<Asset>& rAsset,
		const SAssetMetadata& metadata) const override;

	virtual bool GetInvalidAsset(ZRef<Asset>& asset) override;

private:
	ZRef<Asset> m_InvalidAsset;
};

class ZAssetSerializerSprite : public ZAssetSerializer
{
public:
	virtual void Serialize(const SAssetMetadata& metadata, const ZRef<Asset>& asset) const override
	{
	}

	virtual bool TryLoadData(
		ZRef<Asset>& rAsset,
		const SAssetMetadata& metadata) const override;

	virtual bool GetInvalidAsset(
		ZRef<Asset>& rAsset) override
	{
		return false;
	}
};

class ZAssetSerializerPrefab : public ZAssetSerializer
{
public:
	virtual void Serialize(
		const SAssetMetadata& metadata,
		const ZRef<Asset>& asset) const override;

	virtual bool TryLoadData(
		ZRef<Asset>& rAsset,
		const SAssetMetadata& metadata) const override;

	virtual bool GetInvalidAsset(
		ZRef<Asset>& rAsset) override;

private:
	ZRef<Asset> m_InvalidAsset;
};

}
