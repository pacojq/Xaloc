#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Assets/AssetManager.h"
#include "Xaloc/Core/Assets/AssetType.h"
#include "Xaloc/Files/FileInfo.h"

#include <map>

namespace Xaloc
{
	class AssetEditor
	{
	public:
		AssetEditor() = default;


		virtual void OnImGuiRender();

		virtual void Dispose();


	public:
		static void RegisterDefaultEditors();

		template<typename T>
		static void RegisterEditor(AssetType type)
		{
			static_assert(std::is_base_of<AssetEditor, T>::value, "AssetEditorPanel::RegisterEditor requires template type to inherit from AssetEditor");
			XA_CORE_ASSERT(s_Editors.find(type) == s_Editors.end(), "There's already an editor for that asset!");
			s_Editors[type] = []() { return static_cast<AssetEditor*>(new T()); };
		}

		static AssetEditor* Instantiate(AssetType type)
		{
			return s_Editors[type]();
		}


	private:
		static std::unordered_map<AssetType, AssetEditor* (*)()> s_Editors;
	};

}