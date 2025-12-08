#pragma once
#include "tooling/asseteditor/asseteditor.h"
#include "xaloc/renderer/resource/Texture.h"


namespace Xaloc
{

class ZAssetEditorTexture2D : public ZAssetEditor
{
public:
	ZAssetEditorTexture2D() = default;

	virtual void OnImGuiRender() override;

public:
	void SetAsset(const ZRef<ZTexture2D>& pAsset) { m_pAsset = (ZRef<ZTexture2D>)pAsset; }

private:
	ZRef<ZTexture2D> m_pAsset;
};

}