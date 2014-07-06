#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxApplication.h"
#include "LuxInputHandler.h"

static void GLFWErrorCallbackFunc(int error, const char* description)
{
	LUX_LOG(Lux::logERROR) << "A GLFW Error occurred. " << "Error Code: " << error << ". " << "Error description: " << description;
}

Lux::Application::Application() :
m_Window(0)
{

}

Lux::Application::~Application()
{
	SafePtrDelete(m_Window);
}

bool Lux::Application::Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_GLVerMajor, unsigned int a_GLVerMinor, unsigned int a_AA, TLogLevel a_LogLevel)
{
	// Configure logging system
	FILELog::ReportingLevel() = a_LogLevel;
	FILE* logFile = 0;
	fopen_s(&logFile, "Lux.log", "w");
	Output2FILE::Stream() = logFile;

	LUX_LOG(logINFO) << "Logger started.";

	glfwSetErrorCallback(GLFWErrorCallbackFunc);

	// Init GLFW
	int initResult = glfwInit();
	if (!initResult)
	{
		LUX_LOG(logERROR) << "Failed to initialize GLFW. " << "Error Code: " << initResult;
		return false;
	}

	// Init the Render Window
	m_Window = new RenderWindow();
	bool windowInit = m_Window->Initialize(a_Width, a_Height, a_Caption, a_GLVerMajor, a_GLVerMinor, a_AA);

	if (!windowInit)
	{
		return false;
	}

	return true;
}

void Lux::Application::Terminate()
{
	glfwTerminate();
}

bool Lux::Application::LoadContent()
{
	return true;
}

bool Lux::Application::UnloadContent()
{
	return true;
}

void Lux::Application::Render()
{

}

void Lux::Application::Update()
{
	glfwSwapBuffers(m_Window->GetWindowHandle());
	glfwPollEvents();
}

const bool Lux::Application::ShouldQuit()
{
	bool ret = false;
	if (glfwWindowShouldClose(m_Window->GetWindowHandle()))
	{
		return true;
	}

	return false;
}
