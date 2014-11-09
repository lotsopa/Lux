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

				virtual const bool GetKeyDown(KeyCode a_Key);
				virtual const bool GetKeyUp(KeyCode a_Key);
				virtual const bool GetKey(KeyCode a_Key);
				virtual const vec2 GetCursorPos();
				virtual const bool GetMouseButtonDown(MouseButton a_Button);
				virtual const bool GetMouseButtonUp(MouseButton a_Button);
				virtual const bool GetMouseButton(MouseButton a_Button);

			private:
				RenderWindowOGL* m_GLWindow;
			};
		}
	}
}

#endif