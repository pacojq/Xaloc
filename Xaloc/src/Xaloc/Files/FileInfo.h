#pragma once
#include "AssetType.h"

#include <string>
#include <vector>

namespace Xaloc {

	struct DirectoryInfo
	{
		std::string Name;
		std::string AbsolutePath;
	};
	
	struct FileInfo
	{
		std::string Name;
		std::string Extension;
		AssetType Type;
		std::string AbsolutePath;

		FileInfo(const std::string& fileName, const std::string& fileExtension, const std::string& absolutePath)
		{
			Name = fileName;
			Extension = fileExtension;
			Type = AssetTypes::FromExtension(fileExtension);
			AbsolutePath = absolutePath;
		}
	};

	struct DirectoryContent
	{
		std::vector<DirectoryInfo> Directories;
		std::vector<FileInfo> Files;
	};

}
