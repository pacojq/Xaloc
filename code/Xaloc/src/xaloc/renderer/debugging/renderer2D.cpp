#include "xapch.h"
#include "xaloc/renderer/debugging/renderer2D.h"


namespace Xaloc
{

void ZDebugRenderer2D::ResetStats()
{
	memset(&m_stats, 0, sizeof(SStatsRenderer2D));
}

const SStatsRenderer2D& ZDebugRenderer2D::ReadStats() const
{
	return m_stats;
}

SStatsRenderer2D& ZDebugRenderer2D::WriteStats()
{
	return m_stats;
}

}