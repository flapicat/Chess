#include "LayerStack.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (auto& layer : m_Layers)
		layer->OnDetach();
	m_Layers.clear();
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.insert(m_Layers.end(), layer);
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	m_Layers.erase(it);
}