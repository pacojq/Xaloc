#include "xapch.h"
#include "Component.h"

namespace Xaloc {

	void Component::Bind(GameObject* go)
	{
		// TODO handle if m_GameObject is not null
		m_GameObject = go;
	}
}