#pragma once
#include "xaloc/types.h"
#include <string>


namespace Xaloc
{

class ZStringRef
{
public:
	ZStringRef(const char* sValue);
	ZStringRef(const char* sValue, uint32 nLength);

	ZStringRef(const std::string& sValue);
	ZStringRef(const std::string& sValue, uint32 nLength);
	ZStringRef(const std::string& sValue, uint32 nOffset, uint32 nLength);

	inline int32 Length() const { return m_nLength; }

	bool Equals(const char* sValue) const;

	inline char operator[](uint32 nIndex) const { return m_pBegin[nIndex]; }

private:
	const char* m_pBegin;
	uint32 m_nLength;
};

int32 StringFindFirst(const char* sString, char cValue);

int32 StringFindLast(const char* sString, char cValue);

}