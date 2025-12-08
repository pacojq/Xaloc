#pragma once
#include "xaloc/events/event.h"
#include "glm/gtx/io.hpp"


namespace Xaloc
{

class EvtWindowResize : public ZEvent
{
public:
	EvtWindowResize(uint32 nWidth, uint32 nHeight)
		: m_nWidth(nWidth)
		, m_nHeight(nHeight)
	{
	}

	inline uint32 GetWidth() const { return m_nWidth; }
	inline uint32 GetHeight() const { return m_nHeight; }

	bool IsMinimized() const { return m_nWidth == 0 || m_nHeight == 0; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtWindowResize: " << m_nWidth << ", " << m_nHeight;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)

private:
	uint32 m_nWidth;
	uint32 m_nHeight;
};

class EvtWindowClose : public ZEvent
{
public:
	EvtWindowClose()
	{
	}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)
};

class EvtWindowFocus : public ZEvent
{
public:
	EvtWindowFocus(bool bFocused)
		: m_bFocused(bFocused)
	{
	}

	inline bool IsFocused() const { return m_bFocused; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtWindowFocus: " << m_bFocused;
		return ss.str();
	}

	EVENT_CLASS_TYPE(WindowFocus)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)

private:
	bool m_bFocused;
};

class EvtAppTick : public ZEvent
{
public:
	EvtAppTick()
	{
	}

	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)
};

class EvtAppUpdate : public ZEvent
{
public:
	EvtAppUpdate()
	{
	}

	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)
};

class EvtAppRender : public ZEvent
{
public:
	EvtAppRender()
	{
	}

	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EEventCategoryFlags::Application)
};

}
