#include "xapch.h"
#include "tooling/asseteditor/asseteditor.h"
#include "tooling/asseteditor/texture2D.h"


namespace Xaloc
{

void ZAssetEditor::RegisterDefaultEditors()
{
	//RegisterEditor<MaterialEditor>(AssetType::Material);
	RegisterEditor<ZAssetEditorTexture2D>(EAssetType::Texture2D);
	//RegisterEditor<AudioFileViewer>(AssetType::Audio);
	//RegisterEditor<PrefabEditor>(AssetType::Prefab);
	//RegisterEditor<AnimationControllerEditor>(AssetType::Animation);
}

}