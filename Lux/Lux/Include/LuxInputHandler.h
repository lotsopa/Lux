#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

namespace Lux
{
	class RenderWindow;
	class InputListener;

	class InputHandler
	{
	public:
		static InputHandler& GetInstance();

		void RelayKeyDown(GLFWwindow* a_Window, int a_Key, int a_ScanCode);
		void RelayKeyUp(GLFWwindow* a_Window, int a_Key, int a_ScanCode);
		void RelayMouseButtonDown(GLFWwindow* a_Window, int a_Btn);
		void RelayMouseButtonUp(GLFWwindow* a_Window, int a_Btn);
		void RelayMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y);
		void RelayMouseEnterWindow(GLFWwindow* a_Window);
		void RelayMouseLeaveWindow(GLFWwindow* a_Window);
		void RelayMousePosition(GLFWwindow* a_Window, double a_X, double a_Y);

		void RegisterInputListenerWindow(RenderWindow* a_Window);
		void UnregisterInputListenerWindow(RenderWindow* a_Window);

		bool IsWindowRegistered(RenderWindow* a_Window);

	private:
		InputHandler();
		InputHandler(InputHandler const&);// Don't Implement
		void operator=(InputHandler const&);// Don't implement
		typedef std::map<GLFWwindow*, InputListener*> InputWindowMap;
		InputWindowMap m_InputWindows;

		// GLFW callbacks
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow* window, double x, double y);
		static void CursorEnterCallback(GLFWwindow* window, int val);
		static void CursorPosCallback(GLFWwindow* window, double a_X, double a_Y);

		friend class RenderWindow;
	};
}

#endif