#include "LuxPCH.h"
#include "LuxEventHandler.h"
#include "LuxEventListener.h"
#include "LuxRenderWindow.h"

Lux::EventHandler& Lux::EventHandler::GetInstance()
{
	static EventHandler instance;
	return instance;
}

Lux::EventHandler::EventHandler()
{
	m_InputWindows.clear();
}

void Lux::EventHandler::RegisterInputListenerWindow(RenderWindow* a_Window)
{
	LuxAssert(a_Window);

	if (IsWindowRegistered(a_Window) || !a_Window)
	{
		LUX_LOG(logWARNING) << "Input Handler: Trying to register an already registered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.insert(std::make_pair(a_Window->GetWindowHandle(), a_Window->GetInputListener()));
}

void Lux::EventHandler::UnregisterInputListenerWindow(RenderWindow* a_Window)
{
	LuxAssert(a_Window);

	if (!IsWindowRegistered(a_Window) || !a_Window)
	{
		LUX_LOG(logWARNING) << "Input Handler: Trying to unregister an already unregistered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.erase(a_Window->GetWindowHandle());
}

bool Lux::EventHandler::IsWindowRegistered(RenderWindow* a_Window)
{
	int count = m_InputWindows.count(a_Window->GetWindowHandle());

	if (!count)
	{
		return false;
	}

	return true;
}

void Lux::EventHandler::RelayKeyDown(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyDown(a_Key, a_ScanCode);
}

void Lux::EventHandler::RelayKeyUp(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyUp(a_Key, a_ScanCode);
}

void Lux::EventHandler::RelayMouseButtonDown(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonDown(a_Btn);
}

void Lux::EventHandler::RelayMouseButtonUp(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonUp(a_Btn);
}

void Lux::EventHandler::RelayMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMouseScroll(a_X, a_Y);
}

void Lux::EventHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

void Lux::EventHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

void Lux::EventHandler::CursorEnterCallback(GLFWwindow* window, int val)
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

void Lux::EventHandler::RelayMouseEnterWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseEnterWindow();
}

void Lux::EventHandler::RelayMouseLeaveWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseLeaveWindow();
}

void Lux::EventHandler::CursorPosCallback(GLFWwindow* window, double a_X, double a_Y)
{
	EventHandler::GetInstance().RelayMousePosition(window, a_X, a_Y);
}

void Lux::EventHandler::RelayMousePosition(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMousePositionChanged(a_X, a_Y);
}

void Lux::EventHandler::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	EventHandler::GetInstance().RelayMouseScroll(window, x, y);
}
