#include "xapch.h"
#include "xaloc/core/application.h"
#include "xaloc/filesystem/core.h"
#include "xaloc/filesystem/external.h"
#include <Windows.h>
#include <filesystem>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Xaloc
{

static HWND GetWindowHandle()
{
	ZWindow& rWindow = ZApplication::Get()->GetWindow();
	return glfwGetWin32Window((GLFWwindow*)rWindow.GetNativeWindow());
}

bool FileRevealInExplorer(
	const std::filesystem::path& pathFile)
{
	auto pathAbsolute = std::filesystem::canonical(pathFile);
	if (!FileExists(pathAbsolute))
	{
		return false;
	}
	std::string sCommand = fmt::format("explorer.exe /select,\"{0}\"", pathAbsolute.string());
	system(sCommand.c_str());
	return true;
}

bool DirectoryOpenInExplorer(
	const std::filesystem::path& pathDirectory)
{
	auto pathAbsolute = std::filesystem::canonical(pathDirectory);
	if (!FileExists(pathAbsolute))
	{
		return false;
	}
	ShellExecute(NULL, L"explore", pathAbsolute.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}

bool FileOpenExternally(
	const std::filesystem::path& pathFile)
{
	auto pathAbsolute = std::filesystem::canonical(pathFile);
	if (!FileExists(pathAbsolute))
	{
		return false;
	}
	ShellExecute(NULL, L"open", pathAbsolute.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
}

std::string FileOpenDialog()
{
	return FileOpenDialog("All\0*.*\0");
}

std::string FileOpenDialog(const std::string& sFilter)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)ZApplication::Get()->GetWindow().GetNativeWindow());
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::string();
}

std::string FileSaveDialog()
{
	return FileSaveDialog("All\0*.*\0");
}

std::string FileSaveDialog(const std::string& sFilter)
{
	OPENFILENAMEA ofn;         // common dialog box structure
	CHAR szFile[260] = { 0 };  // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::string();
}

std::filesystem::path DirectoryOpenDialog(
	const char* pathInitialDirectory)
{
	// TODO
	std::filesystem::path result = pathInitialDirectory;
	return result;
}

}