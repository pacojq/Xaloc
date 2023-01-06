#pragma once

#include <chrono>
#include <string>

namespace Xaloc {

	struct FrameProfileData
	{
		FloatingPointMicroseconds Start;
		
		std::chrono::microseconds LogiclElapsedTime;
		std::chrono::microseconds RenderElapsedTime;
		
		// TODO std::chrono::microseconds TotalElapsedTime;
		float TotalElapsedTime;
	};

	
	
	class ImGuiProfiler
	{
	public:

		ImGuiProfiler(uint32_t maxFrameCount = 60);
		~ImGuiProfiler() = default;
		
		
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