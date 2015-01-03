#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxEventListener.h"
#include "LuxEventListenerDX11.h"

Lux::Core::Internal::EventListenerDX11::EventListenerDX11(RenderWindow* a_Owner) : EventListener(a_Owner)
{
	m_DX11Window = (RenderWindowDX11*)a_Owner;
	InitKeymap();
}

Lux::Core::Internal::EventListenerDX11::~EventListenerDX11()
{
	m_KeyMap.clear();
	m_MouseBtnMap.clear();
}

const bool Lux::Core::Internal::EventListenerDX11::GetKeyDown(KeyCode a_Key)
{
	return ( (GetKeyState(m_KeyMap.at(a_Key)) & 0x80000000) > 0 );
}

const bool Lux::Core::Internal::EventListenerDX11::GetKeyUp(KeyCode a_Key)
{
	return ( (GetKeyState(m_KeyMap.at(a_Key)) & 0x80000000) < 0);
}

const bool Lux::Core::Internal::EventListenerDX11::GetKey(KeyCode a_Key)
{
	return (GetAsyncKeyState(m_KeyMap.at(a_Key)) < 0);
}

const Lux::vec2 Lux::Core::Internal::EventListenerDX11::GetCursorPosition()
{
	tagPOINT cursorPoint;
	GetCursorPos(&cursorPoint);
	vec2 currCursorPos = vec2((float)cursorPoint.x, (float)cursorPoint.y);
	return currCursorPos;
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButtonDown(MouseButton a_Button)
{
	return ( (GetKeyState(m_MouseBtnMap.at(a_Button)) & 0x80000000) > 0);
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButtonUp(MouseButton a_Button)
{
	return ( (GetKeyState(m_MouseBtnMap.at(a_Button)) & 0x80000000) < 0) ;
}

const bool Lux::Core::Internal::EventListenerDX11::GetMouseButton(MouseButton a_Button)
{
	return (GetAsyncKeyState(m_MouseBtnMap.at(a_Button)) < 0);
}

void Lux::Core::Internal::EventListenerDX11::InitKeymap()
{
	m_KeyMap.insert(std::make_pair(LUX_KEY_SPACE, VK_SPACE));
	m_KeyMap.insert(std::make_pair(LUX_KEY_APOSTROPHE, VK_OEM_7));
	m_KeyMap.insert(std::make_pair(LUX_KEY_COMMA, VK_OEM_COMMA));
	m_KeyMap.insert(std::make_pair(LUX_KEY_MINUS, VK_OEM_MINUS));
	m_KeyMap.insert(std::make_pair(LUX_KEY_PERIOD, VK_OEM_PERIOD));
	m_KeyMap.insert(std::make_pair(LUX_KEY_SLASH, VK_OEM_102));
	m_KeyMap.insert(std::make_pair(LUX_KEY_0, 0x30));
	m_KeyMap.insert(std::make_pair(LUX_KEY_1, 0x31));
	m_KeyMap.insert(std::make_pair(LUX_KEY_2, 0x32));
	m_KeyMap.insert(std::make_pair(LUX_KEY_3, 0x33));
	m_KeyMap.insert(std::make_pair(LUX_KEY_4, 0x34));
	m_KeyMap.insert(std::make_pair(LUX_KEY_5, 0x35));
	m_KeyMap.insert(std::make_pair(LUX_KEY_6, 0x36));
	m_KeyMap.insert(std::make_pair(LUX_KEY_7, 0x37));
	m_KeyMap.insert(std::make_pair(LUX_KEY_8, 0x38));
	m_KeyMap.insert(std::make_pair(LUX_KEY_9, 0x39));
	m_KeyMap.insert(std::make_pair(LUX_KEY_SEMICOLON, VK_OEM_1));
	m_KeyMap.insert(std::make_pair(LUX_KEY_EQUAL, VK_OEM_PLUS));
	m_KeyMap.insert(std::make_pair(LUX_KEY_A, 0x41));
	m_KeyMap.insert(std::make_pair(LUX_KEY_B, 0x42));
	m_KeyMap.insert(std::make_pair(LUX_KEY_C, 0x43));
	m_KeyMap.insert(std::make_pair(LUX_KEY_D, 0x44));
	m_KeyMap.insert(std::make_pair(LUX_KEY_E, 0x45));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F, 0x46));
	m_KeyMap.insert(std::make_pair(LUX_KEY_G, 0x47));
	m_KeyMap.insert(std::make_pair(LUX_KEY_H, 0x48));
	m_KeyMap.insert(std::make_pair(LUX_KEY_I, 0x49));
	m_KeyMap.insert(std::make_pair(LUX_KEY_J, 0x4A));
	m_KeyMap.insert(std::make_pair(LUX_KEY_K, 0x4B));
	m_KeyMap.insert(std::make_pair(LUX_KEY_L, 0x4C));
	m_KeyMap.insert(std::make_pair(LUX_KEY_M, 0x4D));
	m_KeyMap.insert(std::make_pair(LUX_KEY_N, 0x4E));
	m_KeyMap.insert(std::make_pair(LUX_KEY_O, 0x4F));
	m_KeyMap.insert(std::make_pair(LUX_KEY_P, 0x50));
	m_KeyMap.insert(std::make_pair(LUX_KEY_Q, 0x51));
	m_KeyMap.insert(std::make_pair(LUX_KEY_R, 0x52));
	m_KeyMap.insert(std::make_pair(LUX_KEY_S, 0x53));
	m_KeyMap.insert(std::make_pair(LUX_KEY_T, 0x54));
	m_KeyMap.insert(std::make_pair(LUX_KEY_U, 0x55));
	m_KeyMap.insert(std::make_pair(LUX_KEY_V, 0x56));
	m_KeyMap.insert(std::make_pair(LUX_KEY_W, 0x57));
	m_KeyMap.insert(std::make_pair(LUX_KEY_X, 0x58));
	m_KeyMap.insert(std::make_pair(LUX_KEY_Y, 0x59));
	m_KeyMap.insert(std::make_pair(LUX_KEY_Z, 0x5A));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT_BRACKET, VK_OEM_4));
	m_KeyMap.insert(std::make_pair(LUX_KEY_BACKSLASH, VK_OEM_5));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT_BRACKET, VK_OEM_6));
	m_KeyMap.insert(std::make_pair(LUX_KEY_GRAVE_ACCENT, VK_OEM_7));
	m_KeyMap.insert(std::make_pair(LUX_KEY_WORLD_1, VK_OEM_7));
	m_KeyMap.insert(std::make_pair(LUX_KEY_WORLD_2, VK_OEM_8));
	m_KeyMap.insert(std::make_pair(LUX_KEY_ESCAPE, VK_ESCAPE));
	m_KeyMap.insert(std::make_pair(LUX_KEY_ENTER, VK_RETURN));
	m_KeyMap.insert(std::make_pair(LUX_KEY_TAB, VK_TAB));
	m_KeyMap.insert(std::make_pair(LUX_KEY_BACKSPACE, VK_BACK));
	m_KeyMap.insert(std::make_pair(LUX_KEY_INSERT, VK_INSERT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_DELETE, VK_DELETE));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT, VK_RIGHT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT, VK_LEFT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_DOWN, VK_DOWN));
	m_KeyMap.insert(std::make_pair(LUX_KEY_UP, VK_UP));
	m_KeyMap.insert(std::make_pair(LUX_KEY_PAGE_UP, VK_PRIOR));
	m_KeyMap.insert(std::make_pair(LUX_KEY_PAGE_DOWN, VK_NEXT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_HOME, VK_HOME));
	m_KeyMap.insert(std::make_pair(LUX_KEY_END, VK_END));
	m_KeyMap.insert(std::make_pair(LUX_KEY_CAPS_LOCK, VK_CAPITAL));
	m_KeyMap.insert(std::make_pair(LUX_KEY_SCROLL_LOCK, VK_SCROLL));
	m_KeyMap.insert(std::make_pair(LUX_KEY_NUM_LOCK, VK_NUMLOCK));
	m_KeyMap.insert(std::make_pair(LUX_KEY_PRINT_SCREEN, VK_SNAPSHOT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_PAUSE, VK_MEDIA_PLAY_PAUSE));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F1, VK_F1));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F2, VK_F2));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F3, VK_F3));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F4, VK_F4));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F5, VK_F5));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F6, VK_F6));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F7, VK_F7));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F8, VK_F8));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F9, VK_F9));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F10, VK_F10));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F11, VK_F11));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F12, VK_F12));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F13, VK_F13));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F14, VK_F14));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F15, VK_F15));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F16, VK_F16));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F17, VK_F17));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F18, VK_F18));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F19, VK_F19));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F20, VK_F20));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F21, VK_F21));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F22, VK_F22));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F23, VK_F23));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F24, VK_F24));
	m_KeyMap.insert(std::make_pair(LUX_KEY_F25, VK_F24));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_0, VK_NUMPAD0));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_1, VK_NUMPAD1));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_2, VK_NUMPAD2));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_3, VK_NUMPAD3));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_4, VK_NUMPAD4));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_5, VK_NUMPAD5));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_6, VK_NUMPAD6));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_7, VK_NUMPAD7));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_8, VK_NUMPAD8));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_DECIMAL, VK_SEPARATOR));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_DIVIDE, VK_DIVIDE));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_MULTIPLY, VK_MULTIPLY));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_SUBTRACT, VK_SUBTRACT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_ADD, VK_ADD));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_ENTER, VK_RETURN));
	m_KeyMap.insert(std::make_pair(LUX_KEY_KP_EQUAL, VK_OEM_PLUS));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT_SHIFT, VK_LSHIFT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT_CONTROL, VK_LCONTROL));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT_ALT, VK_MENU));
	m_KeyMap.insert(std::make_pair(LUX_KEY_LEFT_SUPER, VK_LWIN));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT_ALT, VK_RMENU));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT_SHIFT, VK_RSHIFT));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT_CONTROL, VK_RCONTROL));
	m_KeyMap.insert(std::make_pair(LUX_KEY_RIGHT_SUPER, VK_RWIN));
	m_KeyMap.insert(std::make_pair(LUX_KEY_MENU, VK_APPS));

	// Mouse Buttons
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_1, VK_LBUTTON));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_2, VK_RBUTTON));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_3, VK_MBUTTON));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_4, VK_XBUTTON1));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_5, VK_XBUTTON2));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_6, VK_MBUTTON));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_7, VK_XBUTTON1));
	m_MouseBtnMap.insert(std::make_pair(LUX_MOUSE_BUTTON_8, VK_XBUTTON2));

	// TODO - Joystick
}
