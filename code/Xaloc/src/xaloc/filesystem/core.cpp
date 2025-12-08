#include "xapch.h"
#include "xaloc/filesystem/core.h"


namespace Xaloc
{

bool PathIsDirectory(
	const std::filesystem::path& pathDirectory)
{
	return std::filesystem::is_directory(pathDirectory);
}

// whether file A was last modified more recently than file B
bool PathIsNewer(
	const std::filesystem::path& pathA,
	const std::filesystem::path& pathB)
{
	return std::filesystem::last_write_time(pathA) > std::filesystem::last_write_time(pathB);
}

bool DirectoryCreate(
	const std::filesystem::path& pathDirectory)
{
	return std::filesystem::create_directories(pathDirectory);
}

bool DirectoryCreate(
	const std::string& pathDirectory)
{
	return DirectoryCreate(std::filesystem::path(pathDirectory));
}

bool FileExists(
	const std::filesystem::path& pathFile)
{
	return std::filesystem::exists(pathFile);
}

bool FileExists(
	const std::string& pathFile)
{
	return std::filesystem::exists(std::filesystem::path(pathFile));
}

bool FileDelete(
	const std::filesystem::path& pathFile)
{
	if (!FileExists(pathFile))
	{
		return false;
	}

	if (std::filesystem::is_directory(pathFile))
	{
		return std::filesystem::remove_all(pathFile) > 0;
	}
	return std::filesystem::remove(pathFile);
}

bool FileMove(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTarget)
{
	if (FileExists(pathSource))
	{
		return false;
	}
	std::filesystem::rename(pathSource, pathTarget);
	return true;
}

bool FileMoveToDir(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTargetDirectory)
{
	std::filesystem::path pathTarget = pathTargetDirectory / pathSource.filename();
	return FileMove(pathSource, pathTarget);
}

bool FileCopy(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTarget)
{
	if (FileExists(pathSource))
	{
		return false;
	}
	std::filesystem::copy(pathSource, pathTarget);
	return true;
}

bool FileCopyToDir(
	const std::filesystem::path& pathSource,
	const std::filesystem::path& pathTargetDirectory)
{
	std::filesystem::path pathTarget = pathTargetDirectory / pathSource.filename();
	return FileCopy(pathSource, pathTarget);
}

bool FileRename(
	const std::filesystem::path& pathFile,
	const std::string& sNewFileName)
{
	std::string sNameWithExtension = fmt::format("{0}{1}", sNewFileName, pathFile.extension().string());
	std::filesystem::path pathTarget = pathFile.parent_path() / sNameWithExtension;
	return FileMove(pathFile, pathTarget);
}

}
