#include "EditorIcons.h"


namespace Xaloc
{
	EditorIcons::EditorIcons()
	{
		m_IconFolder = Texture2D::Create("assets/editor/asset_icons/folder.png");
		m_IconNone = Texture2D::Create("assets/editor/asset_icons/none.png");
		m_IconFile = Texture2D::Create("assets/editor/asset_icons/file.png");
		m_IconTexture = Texture2D::Create("assets/editor/asset_icons/texture.png");
		m_IconSprite = Texture2D::Create("assets/editor/asset_icons/sprite.png");
		m_IconShader = Texture2D::Create("assets/editor/asset_icons/shader.png");
		m_IconScript = Texture2D::Create("assets/editor/asset_icons/script.png");
		m_IconRoom = Texture2D::Create("assets/editor/asset_icons/room.png");
		m_IconGameObject = Texture2D::Create("assets/editor/asset_icons/gameobject.png");

		m_TexGridView = Texture2D::Create("assets/editor/grid.png");
		m_TexListView = Texture2D::Create("assets/editor/list.png");
		m_TexSearch = Texture2D::Create("assets/editor/search.png");
		m_TexButtonBack = Texture2D::Create("assets/editor/btn-back.png");
		m_TexButtonForward = Texture2D::Create("assets/editor/btn-forward.png");

		m_AssetIconMap[AssetType::None] = m_IconNone;
		m_AssetIconMap[AssetType::Texture2D] = m_IconTexture;
		m_AssetIconMap[AssetType::Sprite] = m_IconSprite;
		m_AssetIconMap[AssetType::Text] = m_IconFile;
		m_AssetIconMap[AssetType::Shader] = m_IconShader;
		m_AssetIconMap[AssetType::Code] = m_IconScript;
		m_AssetIconMap[AssetType::Prefab] = m_IconGameObject;
		m_AssetIconMap[AssetType::Scene] = m_IconRoom;

		m_AssetIconMap[AssetType::Audio] = m_IconFile;
		m_AssetIconMap[AssetType::Material] = m_IconFile;
		m_AssetIconMap[AssetType::Font] = m_IconFile;
		m_AssetIconMap[AssetType::Dll] = m_IconFile;
		m_AssetIconMap[AssetType::Animation] = m_IconFile;
	}

}