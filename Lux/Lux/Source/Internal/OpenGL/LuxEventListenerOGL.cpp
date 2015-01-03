#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxEventListener.h"
#include "LuxRenderWindow.h"
#include "LuxRenderWindowOGL.h"
#include "LuxEventListenerOGL.h"

Lux::Core::Internal::EventListenerOGL::EventListenerOGL(RenderWindow* a_Owner) : EventListener(a_Owner)
{
	m_GLWindow = (RenderWindowOGL*)a_Owner;
}

Lux::Core::Internal::EventListenerOGL::~EventListenerOGL()
{
	m_GLWindow = nullptr;
}

const bool Lux::Core::Internal::EventListenerOGL::GetKeyDown(KeyCode a_Key)
{
	int state = glfwGetKey(m_GLWindow->GetWindowHandle(), a_Key);

	if (state == GLFW_PRESS)
		return true;

	return false;
}

const bool Lux::Core::Internal::EventListenerOGL::GetKeyUp(KeyCode a_Key)
{
	int state = glfwGetKey(m_GLWindow->GetWindowHandle(), a_Key);

	if (state == GLFW_RELEASE)
		return true;

	return false;
}

const Lux::vec2 Lux::Core::Internal::EventListenerOGL::GetCursorPosition()
{
	double x = 0, y = 0;
	glfwGetCursorPos(m_GLWindow->GetWindowHandle(), &x, &y);
	return vec2(x, y);
}

const bool Lux::Core::Internal::EventListenerOGL::GetKey(KeyCode a_Key)
{
	int state = glfwGetKey(m_GLWindow->GetWindowHandle(), a_Key);

	if (state == GLFW_PRESS || state == GLFW_REPEAT)
		return true;

	return false;
}

const bool Lux::Core::Internal::EventListenerOGL::GetMouseButtonDown(MouseButton a_Button)
{
	int state = glfwGetMouseButton(m_GLWindow->GetWindowHandle(), a_Button);

	if (state == GLFW_PRESS)
		return true;

	return false;
}

const bool Lux::Core::Internal::EventListenerOGL::GetMouseButtonUp(MouseButton a_Button)
{
	int state = glfwGetMouseButton(m_GLWindow->GetWindowHandle(), a_Button);

	if (state == GLFW_RELEASE)
		return true;

	return false;
}

const bool Lux::Core::Internal::EventListenerOGL::GetMouseButton(MouseButton a_Button)
{
	int state = glfwGetMouseButton(m_GLWindow->GetWindowHandle(), a_Button);

	if (state == GLFW_PRESS || state == GLFW_REPEAT)
		return true;

	return false;
}
