#pragma once
#include "xaloc/events/event.h"


namespace Xaloc
{

class EvtKey : public ZEvent
{
public:
	inline int32 GetKeyCode() const { return m_nKeyCode; }

	EVENT_CLASS_CATEGORY(
		EEventCategoryFlags::Keyboard |
		EEventCategoryFlags::Input)

protected:
	EvtKey(int keycode)
		: m_nKeyCode(keycode)
	{
	}

	int32 m_nKeyCode;
};

class EvtKeyPressed : public EvtKey
{
public:
	EvtKeyPressed(int32 nKeycode, int32 nRepeatCount)
		: EvtKey(nKeycode)
		, m_nRepeatCount(nRepeatCount)
	{
	}

	inline int GetRepeatCount() const { return m_nRepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtKeyPressed: "
			<< m_nKeyCode
			<< " ("
			<< m_nRepeatCount
			<< " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)

private:
	int m_nRepeatCount;
};

class EvtKeyReleased : public EvtKey
{
public:
	EvtKeyReleased(int32 nKeycode)
		: EvtKey(nKeycode)
	{
	}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtKeyReleased: " << m_nKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class EvtKeyTyped : public EvtKey
{
public:
	EvtKeyTyped(int keycode)
		: EvtKey(keycode)
	{
	}
		
	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "EvtKeyTyped: " << m_nKeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};

}