#include "xapch.h"
#include "tooling/imgui/views/console.h"
#include "tooling/imgui/core.h"
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


namespace Xaloc
{

float ZImGuiConsole::s_DisplayScale = 1.0f;
uint16_t ZImGuiConsole::s_MessageBufferCapacity = 200;
uint16_t ZImGuiConsole::s_MessageBufferSize = 0;
uint16_t ZImGuiConsole::s_MessageBufferBegin = 0;
ZImGuiConsole::Message::Level ZImGuiConsole::s_MessageBufferRenderFilter = ZImGuiConsole::Message::Level::Trace;
std::vector<ZRef<ZImGuiConsole::Message>> ZImGuiConsole::s_MessageBuffer(s_MessageBufferCapacity);
bool ZImGuiConsole::s_AllowScrollingToBottom = true;
bool ZImGuiConsole::s_RequestScrollToBottom = false;

void ZImGuiConsole::AddMessage(ZRef<Message> message)
{
	if (message->m_Level == Message::Level::Invalid)
		return;

	*(s_MessageBuffer.begin() + s_MessageBufferBegin) = message;
	if (++s_MessageBufferBegin == s_MessageBufferCapacity)
		s_MessageBufferBegin = 0;
	if (s_MessageBufferSize < s_MessageBufferCapacity)
		s_MessageBufferSize++;

	// Request to scroll to bottom of the list to view the new message
	s_RequestScrollToBottom = s_AllowScrollingToBottom;
}

void ZImGuiConsole::Clear()
{
	for (auto message = s_MessageBuffer.begin(); message != s_MessageBuffer.end(); message++)
		(*message) = std::make_shared<Message>();
	s_MessageBufferBegin = 0;
}

void ZImGuiConsole::OnImGuiRender(bool* show)
{
	ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_FirstUseEver);
	ImGui::Begin("Console", show);
	{
		ImGuiRenderHeader();
		ImGui::Separator();
		ImGuiRenderMessages();
	}
	ImGui::End();
}

void ZImGuiConsole::ImGuiRenderHeader()
{
	ImGuiStyle& style = ImGui::GetStyle();
	const float spacing = style.ItemInnerSpacing.x;

	// Text change level
	//ImGui::AlignFirstTextHeightToWidgets();
	ImGui::Text("Display");

	ImGui::SameLine(0.0f, 2.0f * spacing);

	// Buttons to quickly change level left
	if (ImGui::ArrowButton("##MessageRenderFilter_L", ImGuiDir_Left))
	{
		s_MessageBufferRenderFilter = Message::GetLowerLevel(s_MessageBufferRenderFilter);
	}

	ImGui::SameLine(0.0f, spacing);

	// Dropdown with levels
	ImGui::PushItemWidth(ImGui::CalcTextSize("Critical").x * 1.36f);
	if (ImGui::BeginCombo(
		"##MessageRenderFilter",
		Message::GetLevelName(s_MessageBufferRenderFilter),
		ImGuiComboFlags_NoArrowButton))
	{
		for (auto level = Message::s_Levels.begin(); level != Message::s_Levels.end(); level++)
		{
			bool is_selected = (s_MessageBufferRenderFilter == *level);
			if (ImGui::Selectable(Message::GetLevelName(*level), is_selected))
				s_MessageBufferRenderFilter = *level;
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

	ImGui::SameLine(0.0f, spacing);

	// Buttons to quickly change level right
	if (ImGui::ArrowButton("##MessageRenderFilter_R", ImGuiDir_Right))
	{
		s_MessageBufferRenderFilter = Message::GetHigherLevel(s_MessageBufferRenderFilter);
	}

	ImGui::SameLine(0.0f, spacing);

	// Button for advanced settings
	ImGui::SameLine(0.0f, ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Settings").x - style.WindowPadding.x / 2.0f);
	if (ImGui::Button("Settings"))
		ImGui::OpenPopup("SettingsPopup");
	if (ImGui::BeginPopup("SettingsPopup"))
	{
		ImGuiRenderSettings();
		ImGui::EndPopup();
	}

}

void ZImGuiConsole::ImGuiRenderSettings()
{
	const float maxWidth = ImGui::CalcTextSize("Scroll to bottom").x * 1.1f;
	const float spacing = ImGui::GetStyle().ItemInnerSpacing.x + ImGui::CalcTextSize(" ").x;

	// Checkbox for scrolling lock
	//ImGui::AlignFirstTextHeightToWidgets();
	ImGui::Text("Scroll to bottom");
	ImGui::SameLine(0.0f, spacing + maxWidth - ImGui::CalcTextSize("Scroll to bottom").x);
	ImGui::Checkbox("##ScrollToBottom", &s_AllowScrollingToBottom);

	ImGui::SameLine(0.0f, spacing);

	// Button to clear the console
	ImGui::SameLine(0.0f, ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Clear console").x + 1.0f);
	if (ImGui::Button("Clear console"))
		ZImGuiConsole::Clear();

	// Slider for font scale
	//ImGui::AlignFirstTextHeightToWidgets();
	ImGui::Text("Display scale");
	ImGui::SameLine(0.0f, spacing + maxWidth - ImGui::CalcTextSize("Display scale").x);
	ImGui::PushItemWidth(maxWidth * 1.25f / 1.1f);
	ImGui::SliderFloat("##DisplayScale", &s_DisplayScale, 0.5f, 4.0f, "%.1f");
	ImGui::PopItemWidth();
}

void ZImGuiConsole::ImGuiRenderMessages()
{

	ImGui::BeginChild("ScrollRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		ImGui::SetWindowFontScale(s_DisplayScale);

		auto messageStart = s_MessageBuffer.begin() + s_MessageBufferBegin;
		if (*messageStart) // If contains old message here
			for (auto message = messageStart; message != s_MessageBuffer.end(); message++)
				(*message)->OnImGuiRender();
		if (s_MessageBufferBegin != 0) // Skipped first messages in vector
			for (auto message = s_MessageBuffer.begin(); message != messageStart; message++)
				(*message)->OnImGuiRender();

		if (s_RequestScrollToBottom && ImGui::GetScrollMaxY() > 0)
		{
			ImGui::SetScrollY(ImGui::GetScrollMaxY());
			s_RequestScrollToBottom = false;
		}
	}
	ImGui::EndChild();
}

std::vector<ZImGuiConsole::Message::Level> ZImGuiConsole::Message::s_Levels{
	ZImGuiConsole::Message::Level::Trace,
	//ZImGuiConsole::Message::Level::Debug,
	ZImGuiConsole::Message::Level::Info,
	ZImGuiConsole::Message::Level::Warn,
	ZImGuiConsole::Message::Level::Error,
	ZImGuiConsole::Message::Level::Critical,
	ZImGuiConsole::Message::Level::Off
};

ZImGuiConsole::Message::Message(const std::string message, Level level)
	: m_Message(message), m_Level(level)
{
}

void ZImGuiConsole::Message::OnImGuiRender()
{
	if (m_Level != Level::Invalid && m_Level >= ZImGuiConsole::s_MessageBufferRenderFilter)
	{
		Color color = GetRenderColor(m_Level);
		ImGui::PushStyleColor(ImGuiCol_Text, { color.r, color.g, color.b, color.a });
		ImGui::TextUnformatted(m_Message.c_str());
		ImGui::PopStyleColor();
	}
}

ZImGuiConsole::Message::Level ZImGuiConsole::Message::GetLowerLevel(Level level)
{
	switch (level)
	{
	case ZImGuiConsole::Message::Level::Off: return ZImGuiConsole::Message::Level::Critical;
	case ZImGuiConsole::Message::Level::Critical: return ZImGuiConsole::Message::Level::Error;
	case ZImGuiConsole::Message::Level::Error: return ZImGuiConsole::Message::Level::Warn;
	case ZImGuiConsole::Message::Level::Warn: //return ZImGuiConsole::Message::Level::Debug;
	case ZImGuiConsole::Message::Level::Debug: return ZImGuiConsole::Message::Level::Info;
	case ZImGuiConsole::Message::Level::Info:
	case ZImGuiConsole::Message::Level::Trace: return ZImGuiConsole::Message::Level::Trace;
	}
	return ZImGuiConsole::Message::Level::Invalid;
}

ZImGuiConsole::Message::Level ZImGuiConsole::Message::GetHigherLevel(Level level)
{
	switch (level)
	{
	case ZImGuiConsole::Message::Level::Trace: return ZImGuiConsole::Message::Level::Info;
	case ZImGuiConsole::Message::Level::Info: //return ZImGuiConsole::Message::Level::Debug;
	case ZImGuiConsole::Message::Level::Debug: return ZImGuiConsole::Message::Level::Warn;
	case ZImGuiConsole::Message::Level::Warn: return ZImGuiConsole::Message::Level::Error;
	case ZImGuiConsole::Message::Level::Error: return ZImGuiConsole::Message::Level::Critical;
	case ZImGuiConsole::Message::Level::Critical:
	case ZImGuiConsole::Message::Level::Off: return ZImGuiConsole::Message::Level::Off;
	}
	return ZImGuiConsole::Message::Level::Invalid;
}


const char* ZImGuiConsole::Message::GetLevelName(Level level)
{
	switch (level)
	{
	case ZImGuiConsole::Message::Level::Trace: return "Trace";
	case ZImGuiConsole::Message::Level::Info: return "Info";
	case ZImGuiConsole::Message::Level::Debug: return "Debug";
	case ZImGuiConsole::Message::Level::Warn: return "Warning";
	case ZImGuiConsole::Message::Level::Error: return "Error";
	case ZImGuiConsole::Message::Level::Critical: return "Critical";
	case ZImGuiConsole::Message::Level::Off: return "None";
	}
	return "Unknown name";
}

ZImGuiConsole::Message::Color ZImGuiConsole::Message::GetRenderColor(Level level)
{
	switch (level)
	{
	case ZImGuiConsole::Message::Level::Trace: return { 0.75f, 0.75f, 0.75f, 1.00f }; // White-ish gray
	case ZImGuiConsole::Message::Level::Info: return { 0.00f, 0.50f, 0.00f, 1.00f }; // Green
	case ZImGuiConsole::Message::Level::Debug: return { 0.00f, 0.50f, 0.50f, 1.00f }; // Cyan
	case ZImGuiConsole::Message::Level::Warn: return { 1.00f, 1.00f, 0.00f, 1.00f }; // Yellow
	case ZImGuiConsole::Message::Level::Error: return { 1.00f, 0.00f, 0.00f, 1.00f }; // Red
	case ZImGuiConsole::Message::Level::Critical: return { 1.00f, 1.00f, 1.00f, 1.00f }; // White-white
	}
	return { 1.00f, 1.00f, 1.00f, 1.00f };
}
}