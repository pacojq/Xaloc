#pragma once

namespace Xaloc {

	class Entity;

	struct IdComponent
	{
		uint32_t ID;

		operator uint32_t () { return ID; }
		operator const uint32_t() const { return ID; }
	};

}