#pragma once

#include "Xaloc/Core/Core.h"
#include "Xaloc/Scene/Scene.h"
#include "Xaloc/Scene/Entity.h"

namespace Xaloc {

	class EditorLayer;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel(EditorLayer* editorLayer, const Ref<Scene>& scene);

		void SetScene(const Ref<Scene>& scene);
		void SetSelected(Entity entity);
		
		Entity GetSelectedEntity() const { return m_SelectionContext; }

		void OnImGuiRender();
		
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
		
	private:
		EditorLayer* m_EditorLayer;
		Ref<Scene> m_Scene;

		bool m_ShowProperties = false;
		Entity m_SelectionContext;
	};

}