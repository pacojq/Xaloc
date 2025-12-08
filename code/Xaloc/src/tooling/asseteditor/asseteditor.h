#pragma once
#include "xaloc/core/core.h"
#include "xaloc/core/assets/asset.h"
#include "xaloc/debugging/assert.h"
#include "xaloc/filesystem/info.h"
#include <map>


namespace Xaloc
{

class ZAssetEditor
{
public:
	ZAssetEditor() = default;

	virtual void OnImGuiRender() = 0;

	virtual void Dispose() { }

public:
	static void RegisterDefaultEditors();

	template<typename T>
	static void RegisterEditor(EAssetType eType)
	{
		static_assert(
			std::is_base_of<ZAssetEditor, T>::value,
			"Template type must inherit from ZAssetEditor");
		XA_CORE_ASSERT(
			s_editors.find(eType) == s_editors.end(),
			"There's already an editor for that asset!");
		s_editors[eType] = []() { return static_cast<ZAssetEditor*>(new T()); };
	}

	static ZAssetEditor* Instantiate(EAssetType eType)
	{
		return s_editors[eType]();
	}

private:
	static std::unordered_map<EAssetType, ZAssetEditor* (*)()> s_editors;
};

}