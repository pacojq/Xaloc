#include "xapch.h"
#include "AssetType.h"

namespace Xaloc {

	AssetType AssetTypes::FromExtension(const std::string& extension)
	{
		for (int i = 0; i < m_ImageFiles.size(); i++)
		{
			if (extension == m_ImageFiles[i])
				return AssetType::Image;
		}

		if (extension == "xaloc")
			return AssetType::Scene;

		if (extension == "mat")
			return AssetType::Material;

		for (int i = 0; i < m_AudioFiles.size(); i++)
		{
			if (extension == m_AudioFiles[i])
				return AssetType::Audio;
		}

		for (int i = 0; i < m_CodeFiles.size(); i++)
		{
			if (extension == m_CodeFiles[i])
				return AssetType::Code;
		}

		for (int i = 0; i < m_ShaderFiles.size(); i++)
		{
			if (extension == m_ShaderFiles[i])
				return AssetType::Shader;
		}
		
		if (extension == "dll")
			return AssetType::Dll;

		if (extension == "ttf")
			return AssetType::Font;

		if (extension == "txt")
			return AssetType::Text;
		
		return AssetType::File;
	};

}