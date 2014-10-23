#include "LuxPCH.h"
#include "LuxEventListener.h"
#include "LuxRenderWindow.h"

Lux::Core::EventListener::EventListener() :
m_WindowOwner(0)
{
	
}

Lux::Core::EventListener::~EventListener()
{

}

void Lux::Core::EventListener::OnKeyDown(int a_Key, int a_ScanCode)
{
	if (a_Key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(m_WindowOwner->GetWindowHandle(), true);
}

void Lux::Core::EventListener::OnKeyUp(int a_Key, int a_ScanCode)
{

}

void Lux::Core::EventListener::OnMouseButtonDown(int a_Btn)
{

}

void Lux::Core::EventListener::OnMouseButtonUp(int a_Btn)
{

}

void Lux::Core::EventListener::OnMouseScroll(double a_OffsetX, double a_OffsetY)
{

}

void Lux::Core::EventListener::SetWindowOwner(RenderWindow* a_Owner)
{
	m_WindowOwner = a_Owner;
}

void Lux::Core::EventListener::OnMouseEnterWindow()
{

}

void Lux::Core::EventListener::OnMouseLeaveWindow()
{

}

void Lux::Core::EventListener::OnMousePositionChanged(double a_X, double a_Y)
{

}
