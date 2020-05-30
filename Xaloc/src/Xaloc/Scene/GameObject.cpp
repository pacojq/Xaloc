#include "xapch.h"
#include "GameObject.h"
#include "Component.h"

namespace Xaloc {


	GameObject::GameObject(const std::string& name)
		: m_Name(name)
	{
		//m_Transform.Position = { 0.0f, 0.0f, 0.0f };
		//m_Transform.Scale = { 1.0f, 1.0f, 1.0f };
		//m_Transform.Rotation = 0;
	}

	GameObject::~GameObject()
	{
		for (Component* component : m_Components)
			delete component;
	}


	void GameObject::OnUpdate(Timestep ts)
	{
		for (Component* component : m_Components)
		{
			component->OnUpdate(ts);
		}
	}


	void GameObject::AddComponent(Component* component)
	{
		component->Bind(this);
		m_Components.push_back(component);
	}
}