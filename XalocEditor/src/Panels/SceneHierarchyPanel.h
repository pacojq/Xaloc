#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Entity.h"

namespace Xaloc {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);
		void SetSelected(Entity entity);

		void OnImGuiRender();
		
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
	private:
		Ref<Scene> m_Scene;

		bool m_ShowProperties = false;
		Entity m_SelectionContext;
	};

}