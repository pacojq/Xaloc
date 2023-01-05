#include "xapch.h"
#include "AssetEditorTexture2D.h"

#include "Xaloc/Editor/PropertyDrawer.h"

#include <imgui/imgui.h>

namespace Xaloc {

	void AssetEditorTexture2D::OnImGuiRender()
	{
		float textureWidth = (float)m_Asset->GetWidth();
		float textureHeight = (float)m_Asset->GetHeight();

		//float bitsPerPixel = Texture::GetBPP(m_Asset->GetFormat());
		float imageSize = ImGui::GetWindowWidth() - 40;
		imageSize = glm::min(imageSize, 100.0f);

		ImGui::SetCursorPosX(20);
		ImGui::Image((ImTextureID)m_Asset->GetRendererID(), { imageSize, imageSize });

		PropertyDrawer::BeginPropertyGrid();
		PropertyDrawer::BeginDisabled();

		PropertyDrawer::Float("Width", textureWidth);
		PropertyDrawer::Float("Height", textureHeight);
		// UI::Property("Bits", bitsPerPixel, 0.1f, 0.0f, 0.0f, true); // TODO: Format

		PropertyDrawer::EndDisabled();
		PropertyDrawer::EndPropertyGrid();
	}

}