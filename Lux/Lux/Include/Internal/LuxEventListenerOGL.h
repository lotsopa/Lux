#ifndef LUX_EVENT_LISTENER_OGL_H
#define LUX_EVENT_LISTENER_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class RenderWindowOGL;
			class EventListenerOGL : public EventListener
			{
			public:
				EventListenerOGL(RenderWindow* a_Owner);
				virtual ~EventListenerOGL();

				virtual const bool GetKeyDown(int a_Key);
				virtual const bool GetKeyUp(int a_Key);
				virtual const bool GetKey(int a_Key);
				virtual const vec2 GetCursorPos(int a_Key);
				virtual const bool GetMouseButtonDown(int a_Button);
				virtual const bool GetMouseButtonUp(int a_Button);
				virtual const bool GetMouseButton(int a_Button);

			private:
				RenderWindowOGL* m_GLWindow;
			};
		}
	}
}

#endif