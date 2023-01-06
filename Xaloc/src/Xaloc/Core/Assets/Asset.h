#pragma once

#include "AssetType.h"

#include "Xaloc/Core/UUID.h"

namespace Xaloc {

	using AssetHandle = UUID;

	class Asset
	{
	public:
		AssetHandle AssetHandle = 0;
		uint16_t AssetFlags = (uint16_t)AssetFlag::None;

	public:
		virtual ~Asset() {}

		static AssetType GetStaticAssetType() { return AssetType::None; }
		virtual AssetType GetAssetType() const = 0;

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