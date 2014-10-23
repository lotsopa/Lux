#ifndef LUX_RENDER_WINDOW_OGL_H
#define LUX_RENDER_WINDOW_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class RenderWindowOGL : public RenderWindow
			{
			public:
				virtual ~RenderWindowOGL();
				virtual bool Initialize(Utility::AppInitOptions& a_AppInitOptions);
				virtual EventListener* GetEventListener();
				virtual void SwapBuffers();
				inline GLFWwindow* GetWindowHandle() { return m_WindowHandle; }

			private:
				RenderWindowOGL();
				GLFWwindow* m_WindowHandle;
				EventListener* m_Input;
				friend class PlatformOGL;
			};
		}
	}
}

#endif