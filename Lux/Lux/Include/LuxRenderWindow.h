#ifndef LUX_RENDER_WINDOW_H
#define LUX_RENDER_WINDOW_H

namespace Lux
{
	namespace Core
	{
		class EventListener;

		class RenderWindow
		{
		public:
			RenderWindow();
			~RenderWindow();

			bool Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_AA);

			inline GLFWwindow* GetWindowHandle() { return m_WindowHandle; }
			inline EventListener* GetInputListener() { return m_Input; }

			void SetInputListener(EventListener* a_Listener);

		private:
			GLFWwindow* m_WindowHandle;
			EventListener* m_Input;

		};
	}
}

#endif