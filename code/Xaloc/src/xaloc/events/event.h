#pragma once
#include "xapch.h"
#include "xaloc/types.h"

/*
Example of an event dispatcher in a ZModule:

	void OnEvent(Xaloc::ZEvent& rEvent) override
	{
		Xaloc::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Xaloc::KeyPressedEvent>(XA_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
	}

	bool OnKeyPressedEvent(Xaloc::KeyPressedEvent& event)
	{
		...
	}
*/

namespace Xaloc
{

enum class EEventType : uint32
{
	None = 0,

	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,

	AppTick,
	AppUpdate,
	AppRender,

	KeyPressed,
	KeyReleased,
	KeyTyped,

	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled
};

enum EEventCategoryFlags : uint8
{
	None        = 0,
	Application = BIT(0),
	Input       = BIT(1),
	Keyboard    = BIT(2),
	Mouse       = BIT(3),
	MouseButton = BIT(4)
};

// Easy and fast way to implement needed functions for an event to be of a type or category

#define EVENT_CLASS_TYPE(type) \
	static EEventType GetStaticType() { return EEventType::type; } \
	virtual EEventType GetEventType() const override { return GetStaticType(); } \
	virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
	virtual uint8 GetCategoryFlags() const override { return (uint8)category; }

class ZEvent
{
public:
	virtual ~ZEvent() = default;

	bool bHandled = false;

	virtual EEventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual uint8 GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EEventCategoryFlags category)
	{
		return GetCategoryFlags() & category;
	}
};

class ZEventDispatcher
{
	template<typename T>
	using FnEventHandler = std::function<bool(T&)>;

public:
	ZEventDispatcher(ZEvent& rEvent)
		: m_rEvent(rEvent)
	{
	}

	template<typename T>
	bool Dispatch(FnEventHandler<T> handler)
	{
		if (m_rEvent.GetEventType() == T::GetStaticType())
		{
			m_rEvent.bHandled |= handler(static_cast<T&>(m_rEvent));
			return true;
		}
		return false;
	}
private:
	ZEvent& m_rEvent;
};

inline std::ostream& operator<<(
	std::ostream& rStream,
	const ZEvent& event)
{
	return rStream << event.ToString();
}

}