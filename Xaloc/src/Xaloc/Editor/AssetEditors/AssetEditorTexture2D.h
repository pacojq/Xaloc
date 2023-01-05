#pragma once

#include "AssetEditor.h"

#include "Xaloc/Renderer/Texture.h"


namespace Xaloc
{
	class AssetEditorTexture2D : public AssetEditor
	{
	public:
		AssetEditorTexture2D() = default;

		virtual void OnImGuiRender() override;

	public:
		void SetAsset(const Ref<Texture2D>& asset) { m_Asset = (Ref<Texture2D>)asset; }

	private:
		Ref<Texture2D> m_Asset;
	};
}