#pragma once

#include <functional>
#include <filesystem>

namespace Xaloc {

	enum class FileSystemAction
	{
		Added, Rename, Modified, Delete
	};

	struct FileSystemChangedEvent
	{
		FileSystemAction Action;
		std::filesystem::path FilePath;
		bool IsDirectory;

		// If this is a rename event the new name will be in the FilePath
		std::wstring OldName = L"";
	};

	class FileSystem
	{

	public:
		static bool CreateDir(const std::filesystem::path& directory);
		static bool CreateDir(const std::string& directory);

		static bool Exists(const std::filesystem::path& filepath);
		static bool Exists(const std::string& filepath);

		static bool Delete(const std::filesystem::path& filepath);

		static bool Move(const std::filesystem::path& filepath, const std::filesystem::path& newFilepath);
		static bool MoveToDir(const std::filesystem::path& filepath, const std::filesystem::path& dir);

		static bool Copy(const std::filesystem::path& filepath, const std::filesystem::path& newFilepath);
		static bool CopyToDir(const std::filesystem::path& filepath, const std::filesystem::path& dir);


		static bool IsDir(const std::filesystem::path& filepath);
		static bool Rename(const std::filesystem::path& filepath, const std::string& newName);

		static bool IsNewer(const std::filesystem::path& fileA, const std::filesystem::path& fileB);


		static bool ShowFileInExplorer(const std::filesystem::path& path);
		static bool OpenDirInExplorer(const std::filesystem::path& path);
		static bool OpenExternally(const std::filesystem::path& path);

	public:
		using FileSystemChangedCallbackFn = std::function<void(const std::vector<FileSystemChangedEvent>&)>;

		static void AddFileSystemChangedCallback(const FileSystemChangedCallbackFn& callback);
		static void StartWatching();
		static void StopWatching();

	private:
		static unsigned long Watch(void* param);
		static std::vector<FileSystemChangedCallbackFn> s_Callbacks;



	public:
		/// <summary>
		/// Pops a "Open File" dialog and returns the selected path.
		/// </summary>
		/// <param name="filter"></param>
		/// <returns></returns>
		static std::string OpenFileDialog(const std::string& filter);
		static inline std::string OpenFileDialog() { return OpenFileDialog("All\0*.*\0"); }

		/// <summary>
		/// Pops a "Save File" dialog and returns the selected path.
		/// </summary>
		/// <param name="filter"></param>
		/// <returns></returns>
		static std::string SaveFileDialog(const std::string& filter);
		static inline std::string SaveFileDialog() { return SaveFileDialog("All\0*.*\0"); }


		static std::filesystem::path OpenFolderDialog(const char* initialFolder = "");
	};

}