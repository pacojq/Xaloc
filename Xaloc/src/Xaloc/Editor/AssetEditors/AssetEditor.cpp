#include "xapch.h"
#include "AssetEditor.h"

#include "AssetEditorTexture2D.h"


namespace Xaloc {

	void AssetEditor::RegisterDefaultEditors()
	{
		//RegisterEditor<MaterialEditor>(AssetType::Material);
		RegisterEditor<AssetEditorTexture2D>(AssetType::Texture2D);
		//RegisterEditor<AudioFileViewer>(AssetType::Audio);
		//RegisterEditor<PrefabEditor>(AssetType::Prefab);
		//RegisterEditor<AnimationControllerEditor>(AssetType::Animation);
	}

}