#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxInputListener.h"
#include "LuxInputHandler.h"

Lux::RenderWindow::RenderWindow() :
m_WindowHandle(0), m_Input(0)
{

}

Lux::RenderWindow::~RenderWindow()
{
	SafePtrDelete(m_Input);
}

bool Lux::RenderWindow::Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_GLVerMajor, unsigned int a_GLVerMinor, unsigned int a_AA)
{
	glfwWindowHint(GLFW_SAMPLES, a_AA);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, a_GLVerMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, a_GLVerMinor);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context 
	m_WindowHandle = glfwCreateWindow(a_Width, a_Height, a_Caption.c_str(), nullptr, nullptr);
	if (m_WindowHandle == nullptr)
	{
		LUX_LOG(logERROR) << "Failed to initialize specified OpenGL " << a_GLVerMajor << "." << a_GLVerMinor << " Context.";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_WindowHandle); // Initialize GLEW 
	int initResult = glewInit();
	if (initResult != GLEW_OK)
	{
		LUX_LOG(logERROR) << "Failed to initialize GLEW. " << "Error code: " << initResult;
		return false;
	}

	// Create a default listener and register it
	m_Input = new InputListener();
	m_Input->SetWindowOwner(this);
	InputHandler::GetInstance().RegisterInputListenerWindow(this);

	// Set up the input callbacks
	glfwSetKeyCallback(m_WindowHandle, InputHandler::KeyCallback);
	glfwSetMouseButtonCallback(m_WindowHandle, InputHandler::MouseButtonCallback);
	glfwSetScrollCallback(m_WindowHandle, InputHandler::MouseScrollCallback);

	glfwSetInputMode(m_WindowHandle, GLFW_STICKY_KEYS, GL_TRUE);

	return true;
}

void Lux::RenderWindow::SetInputListener(InputListener* a_Listener)
{
	LuxAssert(a_Listener);

	if (a_Listener == nullptr)
	{
		LUX_LOG(logWARNING) << "A Render Window Input listener cannot be null. Aborting...";
		return;
	}
	InputHandler::GetInstance().UnregisterInputListenerWindow(this);
	SafePtrDelete(m_Input);
	m_Input = a_Listener;
	m_Input->SetWindowOwner(this);
	InputHandler::GetInstance().RegisterInputListenerWindow(this);
}
