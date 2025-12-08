#pragma once
#include "xaloc/Core/Assets/asset.h"
#include <string>
#include <vector>


namespace Xaloc
{

struct SDirectoryInfo
{
	std::string sName;
	std::string pathAbsolute;
};
	
struct SFileInfo
{
	std::string sName;
	std::string sExtension;
	EAssetType eType;
	std::string pathAbsolute;

	SFileInfo(
		const std::string& sFileName,
		const std::string& sFileExtension,
		const std::string& pathAbsoluteFile);
};

struct SDirectoryContent
{
	std::vector<SDirectoryInfo> aDirectories;
	std::vector<SFileInfo> aFiles;
};

}
