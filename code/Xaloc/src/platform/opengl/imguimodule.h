#pragma once
#include "xaloc/types.h"
#include "tooling/imgui/module.h"

namespace Xaloc
{

class ZModuleImGuiOpenGL : public ZModuleImGui
{
public:
	ZModuleImGuiOpenGL();
	virtual ~ZModuleImGuiOpenGL() = default;

	virtual void Begin() override;
	virtual void End() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

private:
	float32 m_fTime = 0.0f;
};

}
