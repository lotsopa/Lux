#ifndef LUX_EVENT_LISTENER_H
#define LUX_EVENT_LISTENER_H

namespace Lux
{
	namespace Core
	{
		class RenderWindow;
		class EventListener
		{
		public:
			EventListener(RenderWindow* a_Owner);
			virtual ~EventListener();

			// TODO: Finish up Event functions
			virtual const bool GetKeyDown(KeyCode a_Key) = 0;
			virtual const bool GetKeyUp(KeyCode a_Key) = 0;
			virtual const bool GetKey(KeyCode a_Key) = 0;
			virtual const vec2 GetCursorPos() = 0;
			virtual const bool GetMouseButtonDown(MouseButton a_Button) = 0;
			virtual const bool GetMouseButtonUp(MouseButton a_Button) = 0;
			virtual const bool GetMouseButton(MouseButton a_Button) = 0;
		protected:
			RenderWindow* m_WindowOwner;
		};
	}
}

#endif