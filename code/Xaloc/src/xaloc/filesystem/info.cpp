#include "xapch.h"
#include "xaloc/Core/Assets/typeutil.h"
#include "xaloc/filesystem/info.h"


namespace Xaloc
{

SFileInfo::SFileInfo(
	const std::string& sFileName,
	const std::string& sFileExtension,
	const std::string& pathAbsoluteFile)
{
	sName = sFileName;
	sExtension = sFileExtension;
	eType = AssetTypeFromExtension(sFileExtension);
	pathAbsolute = pathAbsoluteFile;
}

}