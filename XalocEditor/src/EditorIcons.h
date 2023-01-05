#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Core/Assets/AssetType.h"

#include "Xaloc/Renderer/Texture.h"

#include <map>

//#include <imgui/imgui.h>
//#include <imgui/imgui_internal.h>

namespace Xaloc
{
	class EditorIcons
	{
	public:
		EditorIcons();
		~EditorIcons() = default;

		//const ImTextureID GetAssetIcon(AssetType type) const
		Ref<Texture2D>& GetAssetIcon(AssetType type) { return m_AssetIconMap[type]; }
		
		Ref<Texture2D>& IconFolder() { return m_IconFolder; }

	private:
		std::map<AssetType, Ref<Texture2D>> m_AssetIconMap;

		Ref<Texture2D> m_IconFolder;
		Ref<Texture2D> m_IconNone;
		Ref<Texture2D> m_IconFile;
		Ref<Texture2D> m_IconTexture;
		Ref<Texture2D> m_IconSprite;
		Ref<Texture2D> m_IconShader;
		Ref<Texture2D> m_IconScript;
		Ref<Texture2D> m_IconRoom;
		Ref<Texture2D> m_IconGameObject;

		Ref<Texture2D> m_TexGridView;
		Ref<Texture2D> m_TexListView;
		Ref<Texture2D> m_TexSearch;
		Ref<Texture2D> m_TexButtonBack;
		Ref<Texture2D> m_TexButtonForward;
	};
}
