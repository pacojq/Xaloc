#pragma once

#include "Xaloc/Core/UUID.h"

namespace Xaloc {

	struct IdComponent
	{
		UUID ID = 0;

		operator UUID () { return ID; }
		operator const UUID() const { return ID; }
	};

}