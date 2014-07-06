#include "LuxPCH.h"
#include "LuxInputHandler.h"
#include "LuxInputListener.h"
#include "LuxRenderWindow.h"

Lux::InputHandler& Lux::InputHandler::GetInstance()
{
	static InputHandler instance;
	return instance;
}

Lux::InputHandler::InputHandler()
{
	m_InputWindows.clear();
}

void Lux::InputHandler::RegisterInputListenerWindow(RenderWindow* a_Window)
{
	LuxAssert(a_Window);

	if (IsWindowRegistered(a_Window) || !a_Window)
	{
		LUX_LOG(logWARNING) << "Input Handler: Trying to register an already registered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.insert(std::make_pair(a_Window->GetWindowHandle(), a_Window->GetInputListener()));
}

void Lux::InputHandler::UnregisterInputListenerWindow(RenderWindow* a_Window)
{
	LuxAssert(a_Window);

	if (!IsWindowRegistered(a_Window) || !a_Window)
	{
		LUX_LOG(logWARNING) << "Input Handler: Trying to unregister an already unregistered or non-existent Render Window. Aborting...";
		return;
	}

	m_InputWindows.erase(a_Window->GetWindowHandle());
}

bool Lux::InputHandler::IsWindowRegistered(RenderWindow* a_Window)
{
	int count = m_InputWindows.count(a_Window->GetWindowHandle());

	if (!count)
	{
		return false;
	}

	return true;
}

void Lux::InputHandler::RelayKeyDown(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyDown(a_Key, a_ScanCode);
}

void Lux::InputHandler::RelayKeyUp(GLFWwindow* a_Window, int a_Key, int a_ScanCode)
{
	m_InputWindows.at(a_Window)->OnKeyUp(a_Key, a_ScanCode);
}

void Lux::InputHandler::RelayMouseButtonDown(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonDown(a_Btn);
}

void Lux::InputHandler::RelayMouseButtonUp(GLFWwindow* a_Window, int a_Btn)
{
	m_InputWindows.at(a_Window)->OnMouseButtonUp(a_Btn);
}

void Lux::InputHandler::RelayMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMouseScroll(a_X, a_Y);
}

void Lux::InputHandler::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		{
			InputHandler::GetInstance().RelayKeyDown(window, key, scancode);
		}
		break;

	case GLFW_RELEASE:
		{
			InputHandler::GetInstance().RelayKeyUp(window, key, scancode);
		}
		break;
	}
}

void Lux::InputHandler::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
	case GLFW_PRESS:
		{
			InputHandler::GetInstance().RelayMouseButtonDown(window, button);
		}
		break;

	case GLFW_RELEASE:
		{
			InputHandler::GetInstance().RelayMouseButtonUp(window, button);
		}
		break;
	}
}

void Lux::InputHandler::CursorEnterCallback(GLFWwindow* window, int val)
{
	switch (val)
	{
	case GL_TRUE:
		InputHandler::GetInstance().RelayMouseEnterWindow(window);
		break;

	case GL_FALSE:
		InputHandler::GetInstance().RelayMouseLeaveWindow(window);
		break;
	}
}

void Lux::InputHandler::RelayMouseEnterWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseEnterWindow();
}

void Lux::InputHandler::RelayMouseLeaveWindow(GLFWwindow* a_Window)
{
	m_InputWindows.at(a_Window)->OnMouseLeaveWindow();
}

void Lux::InputHandler::CursorPosCallback(GLFWwindow* window, double a_X, double a_Y)
{
	InputHandler::GetInstance().RelayMousePosition(window, a_X, a_Y);
}

void Lux::InputHandler::RelayMousePosition(GLFWwindow* a_Window, double a_X, double a_Y)
{
	m_InputWindows.at(a_Window)->OnMousePositionChanged(a_X, a_Y);
}

void Lux::InputHandler::MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	InputHandler::GetInstance().RelayMouseScroll(window, x, y);
}
