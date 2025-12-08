#pragma once
#include "xaloc/types.h"
#include "xaloc/debugging/instrumentor.h"
#include <chrono>
#include <string>
#include <vector>


namespace Xaloc
{

// TODO: make support for Tracy profiler

struct FrameProfileData
{
	FloatingPointMicroseconds Start;

	std::chrono::microseconds LogiclElapsedTime;
	std::chrono::microseconds RenderElapsedTime;

	// TODO std::chrono::microseconds TotalElapsedTime;
	float32 fTotalElapsedTime;
};

class ZImGuiProfiler
{
public:

	ZImGuiProfiler(uint32_t maxFrameCount = 60);
	~ZImGuiProfiler() = default;

	void BeginFrame();
	void EndFrame();

	void OnImGuiRender(bool* show);

private:

	float m_TargetFrameTime = 16.0f;
	uint32_t m_MaxFrameCount;

	std::vector<FrameProfileData> m_Frames;
	FrameProfileData m_CurrentFrame;

	// Values used to plot the lines
	float* m_Xs;
	float* m_YsTotal;
};

}