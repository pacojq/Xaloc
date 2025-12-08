#include "xapch.h"
#include "xaloc/string.h"
#include "xaloc/core/assets/typeutil.h"
#include "xaloc/debugging/assert.h"


namespace Xaloc
{

static const char* s_aTypeNames[] =
{
	"Text",
	"Texture2D",
	"Sprite",
	"Audio",
	"Shader",
	"Material",
	"Animation",
	"Scene",
	"Dll",
	"Font",
	"Prefab"
};

static std::vector<std::string> s_aTypeExtensions[C_ASSET_TYPE_COUNT] =
{
	// Text
	{ "txt" },
	
	// Texture2D
	{"png", "jpg" },
	
	// Sprite
	{ "xaspr" },

	// Audio
	{ "wav", "mp3", "ogg" },
	
	// Shader
	{ "glsl", "hlsl" },

	// Material
	{ "xamat" },

	// Animation
	{ }, // TODO
	
	// Scene
	{ "xaloc" },
	
	// Dll
	{ "dll" },
	
	// Font
	{ "ttf" },
	
	// Prefab
	{ "xaprefab" }
};

std::string AssetTypeToString(EAssetType eType)
{
	return s_aTypeNames[(uint32)eType];
}

EAssetType AssetTypeFromString(const std::string& sType)
{
	if (sType == "Text")      return EAssetType::Text;
	if (sType == "Texture2D") return EAssetType::Texture2D;
	if (sType == "Sprite")    return EAssetType::Sprite;
	if (sType == "Audio")     return EAssetType::Audio;
	if (sType == "Shader")    return EAssetType::Shader;
	if (sType == "Material")  return EAssetType::Material;
	if (sType == "Animation") return EAssetType::Animation;
	if (sType == "Scene")     return EAssetType::Scene;
	if (sType == "Font")      return EAssetType::Font;
	if (sType == "Dll")       return EAssetType::Dll;
	if (sType == "Prefab")    return EAssetType::Prefab;

	XA_CORE_ASSERT(false, "Unknown Asset Type");
	return EAssetType::None;
}

static bool MatchExtension(const ZStringRef& srExtension, uint32 nAssetType)
{
	const std::vector<std::string>& aExtensions = s_aTypeExtensions[nAssetType];
	const uint32 nExtensionCount = aExtensions.size();

	for (uint32 nExtension = 0; nExtension < nExtensionCount; ++nExtension)
	{
		const std::string& sExtension = aExtensions[nExtension];

		if (srExtension.Equals(sExtension.c_str()))
		{
			return true;
		}
	}
	return false;
}

EAssetType AssetTypeFromExtension(const std::string& sExtension)
{
	const uint32 nOffset = sExtension[0] == '.' ? 1 : 0;
	ZStringRef srExtension(sExtension, nOffset, sExtension.length() - nOffset);

	const uint32 nAssetTypes = C_ASSET_TYPE_COUNT;
	for (uint32 nAssetType = 0; nAssetType < nAssetTypes; ++nAssetType)
	{
		if (MatchExtension(srExtension, nAssetType))
		{
			const EAssetType eType = (EAssetType)nAssetType;
			return eType;
		}
	}
	// TODO: message not found?
	return EAssetType::None;
}

}