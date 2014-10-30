#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxEventListener.h"
#include "LuxEventListenerOGL.h"
#include "LuxRenderWindowOGL.h"
#include "LuxErrorCheckOGL.h"

bool Lux::Core::Internal::RenderWindowOGL::Initialize(Utility::AppInitOptions& a_AppInitOptions) 
{
	glfwWindowHint(GLFW_SAMPLES, a_AppInitOptions.m_AntiAliasing);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context 
	m_WindowHandle = glfwCreateWindow(a_AppInitOptions.m_WindowWidth, a_AppInitOptions.m_WindowHeight, a_AppInitOptions.m_WindowCaption.c_str(), nullptr, nullptr);
	if (m_WindowHandle == nullptr)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize specified OpenGL " << 4 << "." << 0 << " Context.";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_WindowHandle);
	Utility::Internal::CheckOGLError();

	// Initialize GLEW 
	glewExperimental = GL_TRUE;
	int initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize GLEW. " << "Error code: " << initResult;
		return false;
	}
	Utility::Internal::ClearAllOGLErrors();

	// Create a default listener
	m_Input = new EventListenerOGL(this);

	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	Utility::Internal::CheckOGLError();
	return true;
}

Lux::Core::EventListener* Lux::Core::Internal::RenderWindowOGL::GetEventListener()
{
	return m_Input;
}

Lux::Core::Internal::RenderWindowOGL::RenderWindowOGL() : m_WindowHandle(0), m_Input(0)
{

}

Lux::Core::Internal::RenderWindowOGL::~RenderWindowOGL()
{
	glfwDestroyWindow(m_WindowHandle);
	Utility::SafePtrDelete(m_Input);
}

void Lux::Core::Internal::RenderWindowOGL::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}

void Lux::Core::Internal::RenderWindowOGL::PollEvents()
{
	glfwPollEvents();
}

