#include "editor/icons.h"

using namespace Xaloc;


namespace Editor
{

ZEditorIcons::ZEditorIcons()
{
	m_IconFolder = ZTexture2D::Create("assets/editor/asset_icons/folder.png");
	m_IconNone = ZTexture2D::Create("assets/editor/asset_icons/none.png");
	m_IconFile = ZTexture2D::Create("assets/editor/asset_icons/file.png");
	m_IconTexture = ZTexture2D::Create("assets/editor/asset_icons/texture.png");
	m_IconSprite = ZTexture2D::Create("assets/editor/asset_icons/sprite.png");
	m_IconShader = ZTexture2D::Create("assets/editor/asset_icons/shader.png");
	m_IconScript = ZTexture2D::Create("assets/editor/asset_icons/script.png");
	m_IconRoom = ZTexture2D::Create("assets/editor/asset_icons/room.png");
	m_IconGameObject = ZTexture2D::Create("assets/editor/asset_icons/gameobject.png");

	m_TexGridView = ZTexture2D::Create("assets/editor/grid.png");
	m_TexListView = ZTexture2D::Create("assets/editor/list.png");
	m_TexSearch = ZTexture2D::Create("assets/editor/search.png");
	m_TexButtonBack = ZTexture2D::Create("assets/editor/btn-back.png");
	m_TexButtonForward = ZTexture2D::Create("assets/editor/btn-forward.png");

	m_AssetIconMap[EAssetType::Text]      = m_IconFile;
	m_AssetIconMap[EAssetType::Texture2D] = m_IconTexture;
	m_AssetIconMap[EAssetType::Sprite]    = m_IconSprite;
	m_AssetIconMap[EAssetType::Audio]     = /* MISSING */ m_IconFile;
	m_AssetIconMap[EAssetType::Shader]    = m_IconShader;
	m_AssetIconMap[EAssetType::Material]  = /* MISSING */ m_IconFile;
	m_AssetIconMap[EAssetType::Animation] = /* MISSING */ m_IconFile;
	m_AssetIconMap[EAssetType::Scene]     = m_IconRoom;
	m_AssetIconMap[EAssetType::Dll]       = m_IconScript;
	m_AssetIconMap[EAssetType::Font]      = /* MISSING */ m_IconFile;
	m_AssetIconMap[EAssetType::Prefab]    = m_IconGameObject;

	m_AssetIconMap[EAssetType::None]      = m_IconNone;
}

}