#include "xapch.h"
#include "AssetManager.h"

namespace Xaloc {

	struct AssetData
	{
		std::unordered_map<std::string, Ref<Texture2D>> Textures;
	};
	static AssetData s_Data;


	void AssetManager::LoadTexture(const std::string& id, const std::string& path)
	{
		// TODO handle repeated IDs
		Ref<Texture2D> tex = Texture2D::Create(path);
		tex->m_AssetID = id;
		s_Data.Textures[id] = tex;
	}


	Ref<Texture2D> AssetManager::GetTexture(const std::string& id)
	{
		// TODO handle errors and return pink texture
		return s_Data.Textures[id];
	}

}