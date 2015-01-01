#ifndef LUX_RENDER_WINDOW_OGL_H
#define LUX_RENDER_WINDOW_OGL_H

namespace Lux
{
	namespace Core
	{
		class SubMesh;
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
				virtual void Clear();
				virtual void PollEvents();
				virtual void Render(SubMesh* a_SubMesh);
				inline GLFWwindow* GetWindowHandle() { return m_WindowHandle; }
				virtual const bool IsWindowResized();
				virtual const int GetHeight();
				virtual const int GetWidth();

			private:
				RenderWindowOGL();
				GLFWwindow* m_WindowHandle;
				EventListenerOGL* m_Input;
				int m_WindowWidth;
				int m_WindowHeight;
				bool m_WindowResized;
				friend class PlatformOGL;
			};
		}
	}
}

#endif