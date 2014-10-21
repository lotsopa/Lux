#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxApplication.h"
#include "LuxEventHandler.h"
#include "LuxEntity.h"
#include "LuxKey.h"
#include "LuxMeshRenderer.h"
#include "LuxTransform.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxTimer.h"

static void ErrorCallbackFunc(int error, const char* description)
{
	LUX_LOG(Lux::Utility::logERROR) << "An Error occurred. " << "Error Code: " << error << ". " << "Error description: " << description;
}

Lux::Application::Application() :
m_Window(nullptr), m_SceneManager(nullptr)
{

}

Lux::Application::~Application()
{
	Utility::SafePtrDelete(m_Window);
	Utility::SafePtrDelete(m_SceneManager);
}

bool Lux::Application::Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_AA, Utility::TLogLevel a_LogLevel)
{
	// Configure logging system
	Utility::FILELog::ReportingLevel() = a_LogLevel;
	FILE* logFile = 0;
	fopen_s(&logFile, "Lux.log", "w");
	Utility::Output2FILE::Stream() = logFile;

	LUX_LOG(Utility::logINFO) << "Logger started.";

	glfwSetErrorCallback(ErrorCallbackFunc);

	// Init GLFW
	int initResult = glfwInit();
	if (!initResult)
	{
		LUX_LOG(Utility::logERROR) << "Failed to initialize GLFW. " << "Error Code: " << initResult;
		return false;
	}

	// Init the Render Window
	m_Window = new Core::RenderWindow();
	bool windowInit = m_Window->Initialize(a_Width, a_Height, a_Caption, a_AA);

	if (!windowInit)
	{
		return false;
	}

	// Init the Scene Manager
	m_SceneManager = new Core::SceneManager(m_Window);
	LoadComponentTypes();
	LoadSystemTypes();
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
	Utility::Timer timer;
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
		InternalUpdate(dt);
		quit = ShouldQuit();
	}
}

void Lux::Application::InternalUpdate(const float a_DeltaTime)
{
	m_SceneManager->ProcessUpdate(a_DeltaTime);
}

void Lux::Application::CheckResult(bool res)
{
	if (!res)
	{
		LUX_LOG(Utility::logERROR) << "Fatal error during program execution. Main loop function returned false. Throwing exception...";
		MessageBox(nullptr, "Fatal error during execution.Aborting program.", "Error", MB_ICONERROR | MB_SETFOREGROUND);
		throw std::invalid_argument("Main loop function returned false.");
	}
}

bool Lux::Application::LoadComponentTypes()
{
	m_SceneManager->RegisterNewComponentType<Core::Transform>();
	m_SceneManager->RegisterNewComponentType<Graphics::MeshRenderer>();
	return true;
}

bool Lux::Application::LoadSystemTypes()
{
	m_SceneManager->RegisterNewSystemType<Graphics::RenderingSystem>();
	m_SceneManager->RegisterComponentTypeWithSystem<Core::Transform, Graphics::RenderingSystem>();
	return true;
}
