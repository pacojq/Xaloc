#pragma once
#include "xaloc/types.h"
#include "xaloc/core/guid.h"


namespace Xaloc
{

	// TODO: migrate to a proper index-based handle type
	using ZAssetHandle = ZGuid;

	enum class AssetFlag : uint8
	{
		None = 0,
		Missing = BIT(0),
		Invalid = BIT(1)
	};

	enum class EAssetType : uint32
	{
		Text,
		Texture2D,
		Sprite,
		Audio,
		Shader,
		Material,
		Animation,
		Scene,
		Dll,
		Font,
		Prefab,

		None // also used for count
	};

	#define C_ASSET_TYPE_COUNT (uint32) EAssetType::None


	// TODO: move all this to asset metadata; we don't need stuff to be inheriting from an "asset" class
	class Asset
	{
	public:
		ZAssetHandle AssetHandle = 0u;
		uint16_t AssetFlags = (uint16_t)AssetFlag::None;

	public:
		virtual ~Asset() {}

		static inline EAssetType GetStaticAssetType() { return EAssetType::None; }
		virtual EAssetType GetAssetType() const = 0;

		bool IsAssetValid() const { return ((AssetFlags & (uint16_t)AssetFlag::Missing) | (AssetFlags & (uint16_t)AssetFlag::Invalid)) == 0; }

		virtual bool operator==(const Asset& other) const
		{
			return AssetHandle == other.AssetHandle;
		}

		virtual bool operator!=(const Asset& other) const
		{
			return !(*this == other);
		}

		bool IsAssetFlagSet(AssetFlag flag) const { return (uint16_t)flag & AssetFlags; }
		void SetAssetFlag(AssetFlag flag, bool value = true)
		{
			if (value)
				AssetFlags |= (uint16_t)flag;
			else
				AssetFlags &= ~(uint16_t)flag;
		}
	};

}