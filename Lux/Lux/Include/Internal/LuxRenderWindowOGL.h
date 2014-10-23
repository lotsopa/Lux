#ifndef LUX_RENDER_WINDOW_OGL_H
#define LUX_RENDER_WINDOW_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class EventListenerOGL;
			class RenderWindowOGL : public RenderWindow
			{
			public:
				virtual ~RenderWindowOGL();
				virtual bool Initialize(Utility::AppInitOptions& a_AppInitOptions);
				virtual EventListener* GetEventListener();
				virtual void SwapBuffers();
				virtual void PollEvents();
				inline GLFWwindow* GetWindowHandle() { return m_WindowHandle; }

			private:
				RenderWindowOGL();
				GLFWwindow* m_WindowHandle;
				EventListenerOGL* m_Input;
				friend class PlatformOGL;
			};
		}
	}
}

#endif