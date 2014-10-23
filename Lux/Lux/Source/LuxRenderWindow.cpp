#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxEventListener.h"
#include "LuxEventHandler.h"

Lux::Core::RenderWindow::RenderWindow() :
m_WindowHandle(0), m_Input(0)
{

}

Lux::Core::RenderWindow::~RenderWindow()
{
	Utility::SafePtrDelete(m_Input);
}

bool Lux::Core::RenderWindow::Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_AA)
{
	glfwWindowHint(GLFW_SAMPLES, a_AA);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context 
	m_WindowHandle = glfwCreateWindow(a_Width, a_Height, a_Caption.c_str(), nullptr, nullptr);
	if (m_WindowHandle == nullptr)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize specified OpenGL " << 4 << "." << 0 << " Context.";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_WindowHandle); 
	// Initialize GLEW 
	glewExperimental = GL_TRUE;
	int initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize GLEW. " << "Error code: " << initResult;
		return false;
	}

	// Create a default listener and register it
	m_Input = new EventListener();
	m_Input->SetWindowOwner(this);
	EventHandler::GetInstance().RegisterInputListenerWindow(this);

	// Set up the input callbacks
	glfwSetKeyCallback(m_WindowHandle, EventHandler::KeyCallback);
	glfwSetMouseButtonCallback(m_WindowHandle, EventHandler::MouseButtonCallback);
	glfwSetScrollCallback(m_WindowHandle, EventHandler::MouseScrollCallback);

	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);

	return true;
}

void Lux::Core::RenderWindow::SetInputListener(EventListener* a_Listener)
{
	LuxAssert(a_Listener);

	if (a_Listener == nullptr)
	{
		LUX_LOG(Utility::logWARNING) << "A Render Window Input listener cannot be null. Aborting...";
		return;
	}
	EventHandler::GetInstance().UnregisterInputListenerWindow(this);
	Utility::SafePtrDelete(m_Input);
	m_Input = a_Listener;
	m_Input->SetWindowOwner(this);
	EventHandler::GetInstance().RegisterInputListenerWindow(this);
}
