#include "xapch.h"
#include "xaloc/types.h"
#include "xaloc/string.h"
#include "xaloc/debugging/assert.h"


namespace Xaloc
{

ZStringRef::ZStringRef(const char* sValue)
	: m_pBegin(sValue)
	, m_nLength(strlen(sValue))
{
}

ZStringRef::ZStringRef(const char* sValue, uint32 nLength)
{
	const uint32 nStringLength = uint32(strlen(sValue));
	XA_CORE_ASSERT(
		nLength <= nStringLength,
		"Cannot make a ZStringRef longer than the referenced string");
	m_pBegin = sValue;
	m_nLength = nLength;
}

ZStringRef::ZStringRef(const std::string& sValue)
	: m_pBegin(sValue.c_str())
	, m_nLength(sValue.length())
{
}

ZStringRef::ZStringRef(const std::string& sValue, uint32 nLength)
{
	const uint32 nStringLength = uint32(sValue.length());
	XA_CORE_ASSERT(
		nLength <= nStringLength,
		"Cannot make a ZStringRef longer than the referenced string");
	m_pBegin = sValue.c_str();
	m_nLength = nLength;
}

ZStringRef::ZStringRef(const std::string& sValue, uint32 nOffset, uint32 nLength)
{
	const uint32 nStringLength = uint32(sValue.length()) - nOffset;
	XA_CORE_ASSERT(
		nStringLength >= 0,
		"Cannot set an offset index out of string bounds");
	XA_CORE_ASSERT(
		nLength <= nStringLength,
		"Cannot make a ZStringRef longer than the referenced string");
	m_pBegin = sValue.c_str() + nOffset;
	m_nLength = nLength;
}

bool ZStringRef::Equals(const char* sValue) const
{
	const char* pValue = m_pBegin;
	uint32 nIndex = 0;
	while (*sValue && (nIndex < m_nLength) && (*pValue == *sValue))
	{
		pValue++;
		sValue++;
		nIndex++;
	}
	return *(const unsigned char*)pValue == *(const unsigned char*)sValue;
}

int32 StringFindFirst(const char* sString, char cValue)
{
	const int32 nLenght = strlen(sString);
	for (int32 nChar = 0; nChar < nLenght; ++nChar)
	{
		if (sString[nChar] == cValue)
		{
			return nChar;
		}
	}
	return -1;
}

int32 StringFindLast(const char* sString, char cValue)
{
	const int32 nLenght = strlen(sString);
	for (int32 nChar = nLenght-1; nChar >= 0; --nChar)
	{
		if (sString[nChar] == cValue)
		{
			return nChar;
		}
	}
	return -1;
}

}