#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxApplication.h"
#include "LuxEventHandler.h"
#include "LuxEntity.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxTransform.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxTimer.h"

static void GLFWErrorCallbackFunc(int error, const char* description)
{
	LUX_LOG(Lux::logERROR) << "A GLFW Error occurred. " << "Error Code: " << error << ". " << "Error description: " << description;
}

Lux::Application::Application() :
m_Window(nullptr), m_SceneManager(nullptr)
{

}

Lux::Application::~Application()
{
	SafePtrDelete(m_Window);
	SafePtrDelete(m_SceneManager);
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

	m_SceneManager = new SceneManager();
	LoadComponentTypes();
	return true;
}

void Lux::Application::Terminate()
{
	glfwTerminate();
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

void Lux::Application::PollEvents()
{
	glfwPollEvents();
}

void Lux::Application::Run()
{
	bool quit = false;
	Timer timer;
	float dt = 0.0f;
	bool result = true;
	// Main loop
	while (!quit)
	{
		dt = (float)timer.GetAsSeconds();
		timer.Reset();
		PollEvents();
		result = Update(dt);
		CheckResult(result);
		Render(dt);
		quit = ShouldQuit();
	}
}

void Lux::Application::Render(const float a_DeltaTime)
{
	glfwSwapBuffers(m_Window->GetWindowHandle());
	bool res = OnFrameStarted(a_DeltaTime);
	CheckResult(res);

	// TODO : Actual rendering

	res = OnFrameEnded(a_DeltaTime);
	CheckResult(res);
}

void Lux::Application::CheckResult(bool res)
{
	if (!res)
	{
		LUX_LOG(logERROR) << "Fatal error during program execution. Main loop function returned false. Throwing exception...";
		MessageBox(nullptr, "Fatal error during execution.Aborting program.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::invalid_argument("Main loop function returned false.");
	}
}

bool Lux::Application::LoadComponentTypes()
{
	m_SceneManager->RegisterNewComponentType<Transform>();
	m_SceneManager->RegisterNewComponentType<Mesh>();
	return true;
}
