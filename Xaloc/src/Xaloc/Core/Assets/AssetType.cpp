#include "xapch.h"
#include "AssetType.h"

namespace Xaloc {


	std::string AssetTypes::TypeToString(AssetType assetType)
	{
		switch (assetType)
		{
		case AssetType::None:					return "None";

		case AssetType::Text:					return "Text";
		case AssetType::Texture2D:				return "Texture2D";
		case AssetType::Sprite:					return "Sprite";
		case AssetType::Audio:					return "Audio";
		case AssetType::Shader:					return "Shader";
		case AssetType::Material:				return "Material";
		case AssetType::Animation:				return "Animation";
		case AssetType::Scene:					return "Scene";
		case AssetType::Code:					return "Code";
		case AssetType::Font:					return "Font";
		case AssetType::Dll:					return "Dll";
		case AssetType::Prefab:					return "Prefab";
		}

		XA_CORE_ASSERT(false, "Unknown Asset Type");
		return "None";
	}

	AssetType AssetTypes::TypeFromString(const std::string& assetType)
	{
		if (assetType == "None")				return AssetType::None;

		if (assetType == "Text")				return AssetType::Text;
		if (assetType == "Texture2D")			return AssetType::Texture2D;
		if (assetType == "Sprite")				return AssetType::Sprite;
		if (assetType == "Audio")				return AssetType::Audio;
		if (assetType == "Shader")				return AssetType::Shader;
		if (assetType == "Material")			return AssetType::Material;
		if (assetType == "Animation")			return AssetType::Animation;
		if (assetType == "Scene")				return AssetType::Scene;
		if (assetType == "Code")				return AssetType::Code;
		if (assetType == "Font")				return AssetType::Font;
		if (assetType == "Dll")					return AssetType::Dll;
		if (assetType == "Prefab")				return AssetType::Prefab;

		XA_CORE_ASSERT(false, "Unknown Asset Type");
		return AssetType::None;
	}

	AssetType AssetTypes::FromExtension(const std::string& extension)
	{
		std::string ext = extension;
		if (ext[0] != '.')
			ext = "." + ext;

		for (int i = 0; i < m_ImageFiles.size(); i++)
		{
			if (extension == m_ImageFiles[i])
				return AssetType::Texture2D;
		}

		if (ext == ".xaloc")
			return AssetType::Scene;

		if (ext == ".xaspr")
			return AssetType::Sprite;

		if (ext == ".xamat")
			return AssetType::Material;

		if (ext == ".xaprefab")
			return AssetType::Prefab;

		for (int i = 0; i < m_AudioFiles.size(); i++)
		{
			if (ext == m_AudioFiles[i])
				return AssetType::Audio;
		}

		for (int i = 0; i < m_CodeFiles.size(); i++)
		{
			if (ext == m_CodeFiles[i])
				return AssetType::Code;
		}

		for (int i = 0; i < m_ShaderFiles.size(); i++)
		{
			if (ext == m_ShaderFiles[i])
				return AssetType::Shader;
		}
		
		if (ext == ".dll")
			return AssetType::Dll;

		if (ext == ".ttf")
			return AssetType::Font;

		if (ext == ".txt")
			return AssetType::Text;
		
		return AssetType::None;
	};

}