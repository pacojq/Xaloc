#include "xapch.h"
#include "tooling/asseteditor/texture2D.h"
#include "tooling/propertydrawer.h"
#include "tooling/imgui/core.h"
#include "xaloc/types.h"


namespace Xaloc
{

void ZAssetEditorTexture2D::OnImGuiRender()
{
	ZPropertyDrawer imgui("AssetEditorTexture2D");

	float32 fTextureWidth = (float)m_pAsset->GetWidth();
	float32 fTextureHeight = (float)m_pAsset->GetHeight();

	float32 fImageSize = ImGui::GetWindowWidth() - 40;
	fImageSize = glm::min(fImageSize, 100.0f);

	ImGui::SetCursorPosX(20);
	ImGui::Image((ImTextureID)m_pAsset->GetRendererID(), { fImageSize, fImageSize });

	imgui.Begin();
	imgui.OpenDisabledScope();

	imgui.Float("Width", fTextureWidth);
	imgui.Float("Height", fTextureHeight);

	imgui.CloseDisabledScope();
	imgui.End();
}

}