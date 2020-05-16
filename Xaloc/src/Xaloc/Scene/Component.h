#pragma once

#include "Xaloc/Core/Timestep.h"

namespace Xaloc {

	class Component
	{
	public:

		virtual void OnUpdate(Timestep ts) = 0;

	private:

	};

}