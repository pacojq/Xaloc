#pragma once

namespace Xaloc {

	class FileUtils
	{
	public:
				
		/// <summary>
		/// Pops a "Open File" dialog and returns the selected path.
		/// </summary>
		/// <param name="filter"></param>
		/// <returns></returns>
		static std::string OpenFileDialog(const std::string& filter);

		static inline std::string OpenFileDialog()
		{
			return OpenFileDialog("All\0*.*\0");
		}

		
		/// <summary>
		/// Pops a "Save File" dialog and returns the selected path.
		/// </summary>
		/// <param name="filter"></param>
		/// <returns></returns>
		static std::string SaveFileDialog(const std::string& filter);

		static inline std::string SaveFileDialog()
		{
			return SaveFileDialog("All\0*.*\0");
		}
	};

}