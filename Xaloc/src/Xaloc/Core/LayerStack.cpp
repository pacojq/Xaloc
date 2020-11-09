#include "xapch.h"
#include "LayerStack.h"


namespace Xaloc {

	LayerStack::LayerStack()
	{
	}


	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	

	void LayerStack::PushLayer_Impl(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay_Impl(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}


	void LayerStack::PopLayer_Impl(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
			layer->OnDetach();
		}
	}

	void LayerStack::PopOverlay_Impl(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			overlay->OnDetach();
		}
	}



	void LayerStack::Flush()
	{
		for (Layer* layer : m_PendingPopOverlay)
			PopOverlay_Impl(layer);
		m_PendingPopOverlay.clear();

		for (Layer* layer : m_PendingPopLayer)
			PopLayer_Impl(layer);
		m_PendingPopLayer.clear();

		for (Layer* layer : m_PendingPushLayer)
			PushLayer_Impl(layer);
		m_PendingPushLayer.clear();

		for (Layer* layer : m_PendingPushOverlay)
			PushOverlay_Impl(layer);
		m_PendingPushOverlay.clear();
	}

}
