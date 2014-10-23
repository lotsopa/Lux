#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxPlatform.h"
#include "LuxRenderWindow.h"
#include "LuxPlatformOGL.h"
#include "LuxRenderWindowOGL.h"

Lux::Core::Internal::PlatformOGL::PlatformOGL()
{

}

Lux::Core::Internal::PlatformOGL::~PlatformOGL()
{
	Utility::SafePtrDelete(m_Window);
}

bool Lux::Core::Internal::PlatformOGL::Init(Utility::AppInitOptions& a_AppInitOptions)
{
	glfwSetErrorCallback(Utility::ErrorCallbackFunc);

	// Init GLFW
	int initResult = glfwInit();
	if (!initResult)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize GLFW. " << "Error Code: " << initResult;
		return false;
	}

	// Init the Render Window
	m_Window = new RenderWindowOGL();
	bool windowInit = m_Window->Initialize(a_AppInitOptions);

	if (!windowInit)
	{
		return false;
	}
	return true;
}

Lux::Core::RenderWindow* Lux::Core::Internal::PlatformOGL::GetRenderWindow()
{
	return m_Window;
}

const bool Lux::Core::Internal::PlatformOGL::ShouldQuit()
{
	if (glfwWindowShouldClose(m_Window->GetWindowHandle()))
	{
		return true;
	}

	return false;
}
