#include "LuxPCH.h"
#include "LuxEventListener.h"
#include "LuxRenderWindow.h"

Lux::EventListener::EventListener() :
m_WindowOwner(0)
{
	
}

Lux::EventListener::~EventListener()
{

}

void Lux::EventListener::OnKeyDown(int a_Key, int a_ScanCode)
{
	if (a_Key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(m_WindowOwner->GetWindowHandle(), true);
}

void Lux::EventListener::OnKeyUp(int a_Key, int a_ScanCode)
{

}

void Lux::EventListener::OnMouseButtonDown(int a_Btn)
{

}

void Lux::EventListener::OnMouseButtonUp(int a_Btn)
{

}

void Lux::EventListener::OnMouseScroll(double a_OffsetX, double a_OffsetY)
{

}

void Lux::EventListener::SetWindowOwner(RenderWindow* a_Owner)
{
	m_WindowOwner = a_Owner;
}

void Lux::EventListener::OnMouseEnterWindow()
{

}

void Lux::EventListener::OnMouseLeaveWindow()
{

}

void Lux::EventListener::OnMousePositionChanged(double a_X, double a_Y)
{

}
