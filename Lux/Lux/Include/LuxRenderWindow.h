#ifndef LUX_RENDER_WINDOW_H
#define LUX_RENDER_WINDOW_H

namespace Lux
{
	class InputListener;

	class RenderWindow
	{
	public:
		RenderWindow();
		~RenderWindow();

		bool Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_GLVerMajor, unsigned int a_GLVerMinor, unsigned int a_AA);

		inline GLFWwindow* GetWindowHandle() { return m_WindowHandle; }
		inline InputListener* GetInputListener() { return m_Input; }

		void SetInputListener(InputListener* a_Listener);

	private:
		GLFWwindow* m_WindowHandle;
		InputListener* m_Input;

	};
}

#endif