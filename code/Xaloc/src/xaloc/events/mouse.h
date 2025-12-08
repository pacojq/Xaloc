#pragma once
#include "xaloc/events/event.h"


namespace Xaloc
{

class EvtMouseMoved : public ZEvent
{
public:
	EvtMouseMoved(float x, float y)
		: m_MouseX(x), m_MouseY(y) {}

	inline float GetX() const { return m_MouseX; }
	inline float GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtMouseMoved: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(
		EEventCategoryFlags::Mouse |
		EEventCategoryFlags::Input)

private:
	float m_MouseX, m_MouseY;
};

class EvtMouseScrolled : public ZEvent
{
public:
	EvtMouseScrolled(float xOffset, float yOffset)
		: m_XOffset(xOffset), m_YOffset(yOffset) {}

	inline float GetXOffset() const { return m_XOffset; }
	inline float GetYOffset() const { return m_YOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtMouseScrolled: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(
		EEventCategoryFlags::Mouse |
		EEventCategoryFlags::Input)
private:
	float m_XOffset, m_YOffset;
};

class EvtMouseButton : public ZEvent
{
public:
	inline int32 GetMouseButton() const { return m_nButton; }

	EVENT_CLASS_CATEGORY(
		EEventCategoryFlags::Mouse |
		EEventCategoryFlags::Input |
		EEventCategoryFlags::MouseButton)

protected:
	EvtMouseButton(int32 nButton)
		: m_nButton(nButton)
	{
	}

	int32 m_nButton;
};

class EvtMouseButtonPressed : public EvtMouseButton
{
public:
	EvtMouseButtonPressed(int32 nButton)
		: EvtMouseButton(nButton)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtMouseButtonPressed: " << m_nButton;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class EvtMouseButtonReleased : public EvtMouseButton
{
public:
	EvtMouseButtonReleased(int32 nButton)
		: EvtMouseButton(nButton)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtMouseButtonReleased: " << m_nButton;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};

}