#pragma once
#include "xaloc/types.h"


namespace Xaloc
{

inline uint32 HashFromString(const char* sValue)
{
	uint32 nResult = 5381;
	unsigned char* pChar = (unsigned char*)sValue;
	while (*pChar != '\0')
	{
		nResult = (nResult << 5) + nResult + *pChar;
		++pChar;
	}
	return nResult;
}

}