#pragma once
#include "xapch.h"
#include "xaloc/types.h"
#include "xaloc/core/core.h"
#include "xaloc/core/log.h"
#include "xaloc/events/event.h"
#include "xaloc/renderer/api.h"
#include "xaloc/renderer/context.h"


namespace Xaloc
{

/*
	Struct holding data about the different window properties.
*/
struct SWindowDefinition
{
	std::string sTitle;
	uint32 nWidth;
	uint32 nHeight;

	bool bVSyncEnabled = true;
	bool bResizable = true;
	bool bDecorated = true;

	SWindowDefinition(const std::string& title = "Xaloc Engine",
					uint32 width = 1280,
					uint32 height = 720)
		: sTitle(title)
		, nWidth(width)
		, nHeight(height)
	{
	}
};

class ZWindow
{
public:
	using FnEventCallback = std::function<void(ZEvent&)>;

	virtual ~ZWindow() = default;

	virtual void OnUpdate() = 0;

	virtual uint32 GetWidth() const = 0;
	virtual uint32 GetHeight() const = 0;

	// Window attributes
	virtual void SetEventCallback(const FnEventCallback& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;

	virtual void* GetNativeWindow() const = 0;

	virtual ZGraphicsContext* GetContext() = 0;

	static ZWindow* Create(
		ERenderAPI eRenderAPI,
		const SWindowDefinition& definition = SWindowDefinition());

public:
	static float s_HighDPIScaleFactor;

};


}
