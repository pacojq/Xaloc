#pragma once
#include "xaloc/types.h"
#include <filesystem>


namespace Xaloc
{

bool PathIsDirectory(
	const std::filesystem::path& pathDirectory);

// whether file A was last modified more recently than file B
bool PathIsNewer(
	const std::filesystem::path& pathA,
	const std::filesystem::path& pathB);

bool FileRevealInExplorer(
	const std::filesystem::path& path);
bool DirectoryOpenInExplorer(
	const std::filesystem::path& path);
bool FileOpenExternally(
	const std::filesystem::path& path);

bool DirectoryCreate(
	const std::filesystem::path& pathDirectory);
bool DirectoryCreate(
	const std::string& pathDirectory);

bool FileExists(
	const std::filesystem::path& pathFile);
bool FileExists(
	const std::string& pathFile);

bool FileDelete(
	const std::filesystem::path& pathFile);

bool FileMove(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTarget);

bool FileMoveToDir(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTargetDirectory);

bool FileCopy(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTarget);

bool FileCopyToDir(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTargetDirectory);

bool FileRename(
	const std::filesystem::path& pathFile,
	const std::string& sNewFileName);

}