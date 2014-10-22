#include "LuxPCH.h"
#include "LuxEventSystem.h"
#include "LuxEventHandler.h"
#include "LuxEventListener.h"
#include "LuxRenderWindow.h"

Lux::Core::EventHandler& Lux::Core::EventHandler::GetInstance()
{
	static EventHandler instance;
	return instance;
}

Lux::Core::EventHandler::EventHandler()
{
	LUX_LOG(Utility::logINFO) << "Event Handler created successfully.";
}

Lux::Core::EventHandler::~EventHandler()
{
	m_InputWindows.clear();
}

void Lux::Core::EventHandler::RegisterEventSystem(EventSystem* a_System)
{
	LuxAssert(a_System);

	if (IsEventSystemRegistered(a_System) || !a_System)
	{
		LUX_LOG(Utility::logWARNING) << "Input Handler: Trying to register an already registered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.insert(std::make_pair(a_System->GetRenderWindow()->GetWindowHandle(), a_System));
}

void Lux::Core::EventHandler::UnregisterEventSystem(EventSystem* a_System)
{
	LuxAssert(a_System);

	if (!IsEventSystemRegistered(a_System) || !a_System)
	{
		LUX_LOG(Utility::logWARNING) << "Input Handler: Trying to unregister an already unregistered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.erase(a_System->GetRenderWindow()->GetWindowHandle());
}

bool Lux::Core::EventHandler::IsEventSystemRegistered(EventSystem* a_System)
{
	int count = m_InputWindows.count(a_System->GetRenderWindow()->GetWindowHandle());

	if (!count)
	{
		return false;
	}

	return true;
}

void Lux::Core::EventHandler::RelayKeyDown(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyDown(a_Key, a_ScanCode);
}

void Lux::Core::EventHandler::RelayKeyUp(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyUp(a_Key, a_ScanCode);
}

void Lux::Core::EventHandler::RelayMouseButtonDown(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonDown(a_Btn);
}

void Lux::Core::EventHandler::RelayMouseButtonUp(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonUp(a_Btn);
}

void Lux::Core::EventHandler::RelayMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMouseScroll(a_X, a_Y);
}

void Lux::Core::EventHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		{
			EventHandler::GetInstance().RelayKeyDown(window, key, scancode);
		}
		break;

	case GLFW_RELEASE:
		{
			EventHandler::GetInstance().RelayKeyUp(window, key, scancode);
		}
		break;
	}
}

void Lux::Core::EventHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		{
			EventHandler::GetInstance().RelayMouseButtonDown(window, button);
		}
		break;

	case GLFW_RELEASE:
		{
			EventHandler::GetInstance().RelayMouseButtonUp(window, button);
		}
		break;
	}
}

void Lux::Core::EventHandler::CursorEnterCallback(GLFWwindow* window, int val)
{
	switch (val)
	{
	case GL_TRUE:
		EventHandler::GetInstance().RelayMouseEnterWindow(window);
		break;

	case GL_FALSE:
		EventHandler::GetInstance().RelayMouseLeaveWindow(window);
		break;
	}
}

void Lux::Core::EventHandler::RelayMouseEnterWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseEnterWindow();
}

void Lux::Core::EventHandler::RelayMouseLeaveWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseLeaveWindow();
}

void Lux::Core::EventHandler::CursorPosCallback(GLFWwindow* window, double a_X, double a_Y)
{
	EventHandler::GetInstance().RelayMousePosition(window, a_X, a_Y);
}

void Lux::Core::EventHandler::RelayMousePosition(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMousePositionChanged(a_X, a_Y);
}

void Lux::Core::EventHandler::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	EventHandler::GetInstance().RelayMouseScroll(window, x, y);
}
