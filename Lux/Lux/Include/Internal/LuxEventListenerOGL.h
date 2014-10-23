#ifndef LUX_EVENT_LISTENER_OGL_H
#define LUX_EVENT_LISTENER_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class EventListenerOGL : public EventListener
			{
			public:
				EventListenerOGL(RenderWindow* a_Owner);
				virtual ~EventListenerOGL();
			private:
			};
		}
	}
}

#endif