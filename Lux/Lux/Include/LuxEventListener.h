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
			virtual const bool GetKeyDown(int a_Key) = 0;
			virtual const bool GetKeyUp(int a_Key) = 0;
			virtual const bool GetKey(int a_Key) = 0;
			virtual const vec2 GetCursorPos(int a_Key) = 0;
			virtual const bool GetMouseButtonDown(int a_Button) = 0;
			virtual const bool GetMouseButtonUp(int a_Button) = 0;
			virtual const bool GetMouseButton(int a_Button) = 0;
		protected:
			RenderWindow* m_WindowOwner;
		};
	}
}

#endif