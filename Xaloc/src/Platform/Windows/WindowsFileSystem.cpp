#include "xapch.h"

#include "Xaloc/Files/FileSystem.h"

#include "Xaloc/Core/Application.h"
#include "Xaloc/Core/Assets/AssetManager.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <Windows.h>

#include <filesystem>


namespace Xaloc {

	std::vector<FileSystem::FileSystemChangedCallbackFn> FileSystem::s_Callbacks;

	static bool s_Watching = false;
	static bool s_IgnoreNextChange = false;
	static HANDLE s_WatcherThread;
	static std::filesystem::path s_PersistentStoragePath;


	void FileSystem::AddFileSystemChangedCallback(const FileSystemChangedCallbackFn& callback)
	{
		s_Callbacks.push_back(callback);
	}

	void FileSystem::StartWatching()
	{
		s_Watching = true;
		DWORD threadId;
		s_WatcherThread = CreateThread(NULL, 0, Watch, 0, 0, &threadId);
		XA_CORE_ASSERT(s_WatcherThread != NULL);
		SetThreadDescription(s_WatcherThread, L"Hazel FileSystemWatcher");
	}

	void FileSystem::StopWatching()
	{
		if (!s_Watching)
			return;

		s_Watching = false;
		//DWORD result = WaitForSingleObject(s_WatcherThread, 5000);
		//if (result == WAIT_TIMEOUT)
		// NOTE(Yan): this is a little annoying, but it's a quick and dirty
		//            way to shutdown the file watching ASAP.
		TerminateThread(s_WatcherThread, 0);
		CloseHandle(s_WatcherThread);
	}


	unsigned long FileSystem::Watch(void* param)
	{
		std::filesystem::path assetDirectory = AssetManager::GetRoot().AbsolutePath;
		std::wstring dirStr = assetDirectory.wstring();

		char buf[2048];
		DWORD bytesReturned;
		std::filesystem::path filepath;
		BOOL result = TRUE;

		HANDLE directoryHandle = CreateFile(
			dirStr.c_str(),
			GENERIC_READ | FILE_LIST_DIRECTORY,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		if (directoryHandle == INVALID_HANDLE_VALUE)
		{
			XA_CORE_ASSERT(false, "Failed to open directory!");
			return 0;
		}

		OVERLAPPED pollingOverlap;
		pollingOverlap.OffsetHigh = 0;
		pollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		std::vector<FileSystemChangedEvent> eventBatch;
		eventBatch.reserve(10);

		while (s_Watching && result)
		{
			result = ReadDirectoryChangesW(
				directoryHandle,
				&buf,
				sizeof(buf),
				TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME |
				FILE_NOTIFY_CHANGE_DIR_NAME |
				FILE_NOTIFY_CHANGE_SIZE,
				&bytesReturned,
				&pollingOverlap,
				NULL
			);

			WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

			if (s_IgnoreNextChange)
			{
				s_IgnoreNextChange = false;
				eventBatch.clear();
				continue;
			}

			FILE_NOTIFY_INFORMATION* pNotify;
			int offset = 0;
			std::wstring oldName;

			do
			{
				pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
				size_t filenameLength = pNotify->FileNameLength / sizeof(wchar_t);

				FileSystemChangedEvent e;
				e.FilePath = std::filesystem::path(std::wstring(pNotify->FileName, filenameLength));
				e.IsDirectory = IsDir(e.FilePath);

				switch (pNotify->Action)
				{
				case FILE_ACTION_ADDED:
				{
					e.Action = FileSystemAction::Added;
					break;
				}
				case FILE_ACTION_REMOVED:
				{
					e.Action = FileSystemAction::Delete;
					break;
				}
				case FILE_ACTION_MODIFIED:
				{
					e.Action = FileSystemAction::Modified;
					break;
				}
				case FILE_ACTION_RENAMED_OLD_NAME:
				{
					oldName = e.FilePath.filename();
					break;
				}
				case FILE_ACTION_RENAMED_NEW_NAME:
				{
					e.OldName = oldName;
					e.Action = FileSystemAction::Rename;
					break;
				}
				}

				// NOTE(Peter): Fix for https://gitlab.com/chernoprojects/Hazel-dev/-/issues/143
				bool hasAddedEvent = false;
				if (e.Action == FileSystemAction::Modified)
				{
					for (const auto& event : eventBatch)
					{
						if (event.FilePath == e.FilePath && event.Action == FileSystemAction::Added)
							hasAddedEvent = true;
					}
				}

				if (pNotify->Action != FILE_ACTION_RENAMED_OLD_NAME && !hasAddedEvent)
					eventBatch.push_back(e);

				offset += pNotify->NextEntryOffset;
			} while (pNotify->NextEntryOffset);

			if (eventBatch.size() > 0)
			{
				for (auto& callback : s_Callbacks)
					callback(eventBatch);
				eventBatch.clear();
			}
		}

		CloseHandle(directoryHandle);
		return 0;
	}











	// TODO Move this somewhere else
	std::string FileSystem::OpenFileDialog(const std::string& filter)
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
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

	std::string FileSystem::SaveFileDialog(const std::string& filter)
	{
		OPENFILENAMEA ofn;       // common dialog box structure
		CHAR szFile[260] = { 0 };       // if using TCHAR macros

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get().GetWindow().GetNativeWindow());
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

	std::filesystem::path FileSystem::OpenFolderDialog(const char* initialFolder)
	{
		// TODO
		std::filesystem::path result = initialFolder;
		return result;
	}


}