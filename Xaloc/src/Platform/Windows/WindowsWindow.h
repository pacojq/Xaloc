#pragma once

#include "Xaloc/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Xaloc/Renderer/GraphicsContext.h"

namespace Xaloc {


	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
	
		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override {
			m_Data.EventCallback = callback;
		};
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline void* GetNativeWindow() const override { return m_Window; }

		inline virtual GraphicsContext* GetContext() const override  { return m_Context.get(); }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

	};


}
