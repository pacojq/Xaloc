#pragma once
#include "xaloc/core/core.h"
#include <xhash>


namespace Xaloc
{

// Randomly generated 64-bit integer, as the
// possibility of a clash is low enough.
// This may change in the future.
class ZGuid
{
public:
	ZGuid();
	ZGuid(uint64_t nValue);
	ZGuid(const ZGuid& nGuid);

	inline uint64_t Value() const { return m_nValue; }

	operator uint64_t () { return m_nValue; }
	operator const uint64_t() const { return m_nValue; }
private:
	uint64_t m_nValue;
};

}

namespace std
{

template <>
struct hash<Xaloc::ZGuid>
{
	std::size_t operator()(const Xaloc::ZGuid& nGuid) const
	{
		return hash<uint64_t>()((uint64_t)nGuid);
	}
};
}