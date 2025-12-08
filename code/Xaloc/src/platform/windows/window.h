#pragma once
#include "xaloc/core/window.h"
#include "xaloc/renderer/context.h"
#include "xaloc/renderer/api.h"
#include <GLFW/glfw3.h>


namespace Xaloc
{

struct SWindowsWindow
{
	std::string sTitle;
	uint32 nWidth;
	uint32 nHeight;
	bool bVSync;

	ZWindow::FnEventCallback EventCallback;
};

class ZWindowsWindow : public ZWindow
{
public:
	ZWindowsWindow(
		ERenderAPI eRenderAPI,
		const SWindowDefinition& definition);
	virtual ~ZWindowsWindow();

	void OnUpdate() override;

	inline unsigned int GetWidth() const override { return m_Data.nWidth; }
	inline unsigned int GetHeight() const override { return m_Data.nHeight; }
	
	inline void SetEventCallback(const FnEventCallback& callback) override
	{
		m_Data.EventCallback = callback;
	};

	void SetVSync(bool enabled) override;
	bool IsVSync() const override;

	inline void* GetNativeWindow() const override { return m_Window; }

	inline virtual ZGraphicsContext* GetContext() override { return m_pContext; }

private:
	void Init(
		ERenderAPI eRenderAPI,
		const SWindowDefinition& definition);

	void Shutdown();

private:
	GLFWwindow* m_Window;
	ZGraphicsContext* m_pContext;

	SWindowsWindow m_Data;
};

}
