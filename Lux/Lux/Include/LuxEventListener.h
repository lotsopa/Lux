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

			// TODO: Event functions

		protected:
			RenderWindow* m_WindowOwner;
		};
	}
}

#endif