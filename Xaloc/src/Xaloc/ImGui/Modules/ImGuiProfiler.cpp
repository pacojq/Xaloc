#include "xapch.h"
#include "ImGuiProfiler.h"


#include "Xaloc/Core/Application.h"

#include <imgui.h>
#include <implot.h>

#include "imgui_internal.h"

namespace Xaloc {

	
	ImGuiProfiler::ImGuiProfiler(uint32_t maxFrameCount)
		: m_MaxFrameCount(maxFrameCount)
	{
		m_Xs = new float[maxFrameCount]();
		
		m_YsTotal = new float[maxFrameCount]();

		for (int i = 0; i < maxFrameCount; i++)
		{
			m_Xs[i] = i;

			m_YsTotal[i] = 0;
		}
	}


	
	void ImGuiProfiler::BeginFrame()
	{
		// TODO
		m_CurrentFrame = {}; // TODO
	}

	void ImGuiProfiler::EndFrame()
	{
		// TODO
		
		uint32_t size = m_Frames.size();
		if (size >= m_MaxFrameCount)
			m_Frames.erase(m_Frames.begin());

		// TODO
		m_CurrentFrame.TotalElapsedTime = Application::GetFrameTime();
		
		m_Frames.push_back(m_CurrentFrame);
	}

	void ImGuiProfiler::OnImGuiRender(bool* show)
	{
		ImGui::Begin("Profiler", show);

		// TODO 
		// when paused, we must be able to select a given frame
		// and get its chrome:trace data

		
		if (Application::IsPaused())
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::Button("Pause");
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
			
			ImGui::SameLine();

			if (ImGui::Button("Resume"))
			{
				Application::SetPaused(false);
			}
		}
		else
		{
			if (ImGui::Button("Pause"))
			{
				Application::SetPaused(true);
			}

			ImGui::SameLine();

			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			ImGui::Button("Resume");
			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		ImGui::Separator();




		float avg = 0;
		float min = 10000;
		float max = 0;

		for (int i = 0; i < m_Frames.size(); i++)
		{
			float value = m_Frames[i].TotalElapsedTime;

			m_YsTotal[i] = value;
			avg += value;

			if (value < min) min = value;
			if (value > max) max = value;
		}
		avg /= m_Frames.size();

		/*
		int plotMax = max > m_TargetFrameTime * 2.0f ? max * 1.1f : m_TargetFrameTime * 2.0f;

		ImPlot::SetNextAxesLimits(0, m_MaxFrameCount - 1, 0, plotMax, ImGuiCond_Always);

		if (ImPlot::BeginPlot("Frame Time", "Frame", "ms"))
		{
			ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.5f);
			ImPlot::PlotShaded("Current", m_Xs, m_YsTotal, m_MaxFrameCount);
			ImPlot::PopStyleVar();
			ImPlot::EndPlot();
		}
		*/

		ImGui::Separator();

		ImGui::Text("FPS: %f", 1.0f / (avg / 1000.0f));
		ImGui::Text("Frame time (ms): %f", avg); // 1.0f / avg * 1000.0f);
		
		ImGui::End();
	}
	
}
