#pragma once
#include "xaloc/core/guid.h"


namespace Xaloc
{

struct CmpId
{
	ZGuid idValue = 0;

	operator ZGuid () { return idValue; }
	operator const ZGuid() const { return idValue; }
};

}