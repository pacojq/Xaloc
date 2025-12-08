#pragma once
#include "xaloc/types.h"

#define RENDER_STATS_ENABLED 1


namespace Xaloc
{

#if RENDER_STATS_ENABLED

struct SStatsRenderer2D
{
	uint32 nDrawCalls = 0;
	uint32 nQuadCount = 0;

	uint32 GetTotalVertexCount() const { return nQuadCount * 4; }
	uint32 GetTotalIndexCount() const { return nQuadCount * 6; }
};

class ZDebugRenderer2D
{
public:
	void ResetStats();

	const SStatsRenderer2D& ReadStats() const;
	SStatsRenderer2D& WriteStats();

private:
	SStatsRenderer2D m_stats;
};

#else

class ZDebugRenderer2D
{
	// Empty debug
}

#endif

}