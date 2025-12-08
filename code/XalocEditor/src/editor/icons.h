#pragma once
#include "xaloc/core/core.h"
#include "xaloc/core/assets/asset.h"
#include "xaloc/renderer/resource/texture.h"
#include <map>

using namespace Xaloc;


namespace Editor
{

class ZEditorIcons
{
public:
	ZEditorIcons();
	~ZEditorIcons() = default;

	//const ImTextureID GetAssetIcon(AssetType type) const
	ZRef<ZTexture2D>& GetAssetIcon(EAssetType type) { return m_AssetIconMap[type]; }
		
	ZRef<ZTexture2D>& IconFolder() { return m_IconFolder; }

private:
	std::map<EAssetType, ZRef<ZTexture2D>> m_AssetIconMap;

	ZRef<ZTexture2D> m_IconFolder;
	ZRef<ZTexture2D> m_IconNone;
	ZRef<ZTexture2D> m_IconFile;
	ZRef<ZTexture2D> m_IconTexture;
	ZRef<ZTexture2D> m_IconSprite;
	ZRef<ZTexture2D> m_IconShader;
	ZRef<ZTexture2D> m_IconScript;
	ZRef<ZTexture2D> m_IconRoom;
	ZRef<ZTexture2D> m_IconGameObject;

	ZRef<ZTexture2D> m_TexGridView;
	ZRef<ZTexture2D> m_TexListView;
	ZRef<ZTexture2D> m_TexSearch;
	ZRef<ZTexture2D> m_TexButtonBack;
	ZRef<ZTexture2D> m_TexButtonForward;
};

}
