#pragma once
#include "xaloc/types.h"
#include <filesystem>


namespace Xaloc
{

bool FileRevealInExplorer(
	const std::filesystem::path& pathFile);

bool DirectoryOpenInExplorer(
	const std::filesystem::path& pathDirectory);

bool FileOpenExternally(
	const std::filesystem::path& pathFile);

/// <summary>
/// Pops a "Open File" dialog and returns the selected path.
/// </summary>
/// <param name="filter"></param>
/// <returns></returns>
std::string FileOpenDialog();

std::string FileOpenDialog(
	const std::string& sFilter);

/// <summary>
/// Pops a "Save File" dialog and returns the selected path.
/// </summary>
/// <param name="filter"></param>
/// <returns></returns>
std::string FileSaveDialog();

std::string FileSaveDialog(
	const std::string& sFilter);

std::filesystem::path DirectoryOpenDialog(
	const char* pathInitialDirectory = "");

}