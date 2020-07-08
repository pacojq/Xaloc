#include "xapch.h"
#include "RenderCommand.h"


namespace Xaloc {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
	
}