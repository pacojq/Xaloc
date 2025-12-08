#pragma once
#include "xaloc/filesystem/core.h"
#include <functional>


namespace Xaloc
{

enum class EFileSystemAction : uint8
{
	Add,
	Rename,
	Modify,
	Delete
};

struct EvtFileChanged
{
	EFileSystemAction eAction;
	std::filesystem::path path;
	bool bIsDirectory;

	// TODO: change this into: void* pPayload
	// If this is a rename event the new name will be in the FilePath
	std::wstring sNamePrevious = L"";
};

using FnFileChangedCallback = std::function<void(const std::vector<EvtFileChanged>&)>;

class ZFileWatch
{
public:

	static void RegisterCallback(const FnFileChangedCallback& callback);
	static void StartWatching();
	static void StopWatching();

private:
	static unsigned long Watch(void* param);
	static std::vector<FnFileChangedCallback> s_Callbacks;
};

}