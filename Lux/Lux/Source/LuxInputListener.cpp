#include "LuxPCH.h"
#include "LuxInputListener.h"
#include "LuxRenderWindow.h"

Lux::InputListener::InputListener() :
m_WindowOwner(0)
{

}

Lux::InputListener::~InputListener()
{

}

void Lux::InputListener::OnKeyDown(int a_Key, int a_ScanCode)
{
	if (a_Key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(m_WindowOwner->GetWindowHandle(), true);
}

void Lux::InputListener::OnKeyUp(int a_Key, int a_ScanCode)
{

}

void Lux::InputListener::OnMouseButtonDown(int a_Btn)
{

}

void Lux::InputListener::OnMouseButtonUp(int a_Btn)
{

}

void Lux::InputListener::OnMouseScroll(double a_OffsetX, double a_OffsetY)
{

}

void Lux::InputListener::SetWindowOwner(RenderWindow* a_Owner)
{
	m_WindowOwner = a_Owner;
}

void Lux::InputListener::OnMouseEnterWindow()
{

}

void Lux::InputListener::OnMouseLeaveWindow()
{

}

void Lux::InputListener::OnMousePositionChanged(double a_X, double a_Y)
{

}
