#ifndef LUX_EVENT_LISTENER_DX11_H
#define LUX_EVENT_LISTENER_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class RenderWindowDX11;
			class EventListenerDX11 : public EventListener
			{
			public:
				EventListenerDX11(RenderWindow* a_Owner);
				virtual ~EventListenerDX11();
				virtual const bool GetKeyDown(KeyCode a_Key);
				virtual const bool GetKeyUp(KeyCode a_Key);
				virtual const bool GetKey(KeyCode a_Key);
				virtual const vec2 GetCursorPos();
				virtual const bool GetMouseButtonDown(MouseButton a_Button);
				virtual const bool GetMouseButtonUp(MouseButton a_Button);
				virtual const bool GetMouseButton(MouseButton a_Button);


			private:
				RenderWindowDX11* m_DX11Window;
			};
		}
	}
}

#endif