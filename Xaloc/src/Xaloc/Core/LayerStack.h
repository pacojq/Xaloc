#pragma once

#include "Xaloc/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Xaloc {

	/*
		The Layer Stack is where Layers live inside our application.
	*/
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		inline void PushLayer(Layer* layer) { m_PendingPushLayer.push_back(layer); }
		inline void PushOverlay(Layer* overlay) { m_PendingPushOverlay.push_back(overlay); }
				
		inline void PopLayer(Layer* layer) { m_PendingPopLayer.push_back(layer); }
		inline void PopOverlay(Layer* overlay) { m_PendingPopOverlay.push_back(overlay); }

		/// <summary>
		/// Pushes and pops the pending layers.
		/// </summary>
		void Flush();

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		void PushLayer_Impl(Layer* layer);
		void PushOverlay_Impl(Layer* overlay);

		void PopLayer_Impl(Layer* layer);
		void PopOverlay_Impl(Layer* overlay);
		
		
	private:
		std::vector<Layer*> m_Layers;
		
		std::vector<Layer*> m_PendingPushLayer;
		std::vector<Layer*> m_PendingPushOverlay;
		std::vector<Layer*> m_PendingPopLayer;
		std::vector<Layer*> m_PendingPopOverlay;
		
		unsigned int m_LayerInsertIndex = 0;
	};


}
