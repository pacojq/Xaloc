#pragma once
#include "xaloc/core/core.h"
#include "xaloc/scene/entity.h"
#include "xaloc/scene/scene.h"

namespace Xaloc {
	class ZScene;
}

namespace Editor {
	class ZModuleEditor;
}

using namespace Xaloc;


namespace Editor
{

class ZPanelSceneHierarchy
{
public:
	ZPanelSceneHierarchy(ZModuleEditor* pEditor, const ZRef<ZScene>& scene);

	void SetScene(const ZRef<ZScene>& scene);
	void SetSelected(ZEntity entity);

	inline ZEntity GetSelectedEntity() const { return m_SelectionContext; }

	void OnImGuiRender();

	void DrawComponent(CmpTransform& rTransform, ZEntity entity);
	void DrawComponent(CmpScene& rScene, ZEntity entity);
	void DrawComponent(CmpCollider& rCollider, ZEntity entity);
	void DrawComponent(CmpSpriteRenderer& rSpriteRenderer, ZEntity entity);
	void DrawComponent(CmpBehaviour& rBehaviour, ZEntity entity);

private:
	void DrawEntityNode(ZEntity entity);
	void DrawComponents(ZEntity entity);

private:
	ZModuleEditor* m_pEditor;
	ZRef<ZScene> m_Scene;

	bool m_ShowProperties = false;
	ZEntity m_SelectionContext;
};

}