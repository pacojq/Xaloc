#pragma once

#include "xapch.h"

#include "Xaloc/Core/Core.h"
#include "Xaloc/Events/Event.h"
#include "Xaloc/Core/Log.h"
#include "Xaloc/Renderer/GraphicsContext.h"

namespace Xaloc {

	/*
		Struct holding data about the different window properties.
	*/
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		bool IsVSyncEnabled = true;
		bool IsResizable = true;
		bool IsDecorated = true;

		WindowProps(const std::string& title = "Xaloc Engine",
						uint32_t width = 1280,
						uint32_t height = 720)
			: Title(title), Width(width), Height(height)
		{

		}
	};

	/*
		Interface representing a desktop system based window
	*/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		
		virtual GraphicsContext* GetContext() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	public:
		static float s_HighDPIScaleFactor;

	};


}
