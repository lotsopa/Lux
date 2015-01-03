#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxEventListener.h"
#include "LuxEventListenerDX11.h"

Lux::Core::Internal::EventListenerDX11::EventListenerDX11(RenderWindow* a_Owner) : EventListener(a_Owner)
{
	m_DX11Window = (RenderWindowDX11*)a_Owner;
}

Lux::Core::Internal::EventListenerDX11::~EventListenerDX11()
{

}

const bool Lux::Core::Internal::EventListenerDX11::GetKeyDown(KeyCode a_Key)
{
	return false;
}

const bool Lux::Core::Internal::EventListenerDX11::GetKeyUp(KeyCode a_Key)
{
	return false;
}

const bool Lux::Core::Internal::EventListenerDX11::GetKey(KeyCode a_Key)
{
	return false;
}

const Lux::vec2 Lux::Core::Internal::EventListenerDX11::GetCursorPos()
{
	return vec2();
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButtonDown(MouseButton a_Button)
{
	return false;
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButtonUp(MouseButton a_Button)
{
	return false;
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButton(MouseButton a_Button)
{
	return false;
}
