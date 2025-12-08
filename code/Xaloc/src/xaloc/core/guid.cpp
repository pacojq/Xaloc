#include "xapch.h"
#include "guid.h"
#include <random>


namespace Xaloc
{

static std::random_device s_RandomDevice;
static std::mt19937_64 eng(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

ZGuid::ZGuid()
	: m_nValue(s_UniformDistribution(eng))
{
}

ZGuid::ZGuid(uint64_t nValue)
	: m_nValue(nValue)
{
}

ZGuid::ZGuid(const ZGuid& nGuid)
	: m_nValue(nGuid.m_nValue)
{
}

}