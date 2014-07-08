#ifndef LUX_EVENT_HANDLER_H
#define LUX_EVENT_HANDLER_H

namespace Lux
{
	class RenderWindow;
	class EventListener;

	class EventHandler
	{
	public:
		static EventHandler& GetInstance();
		~EventHandler();

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
		EventHandler();
		EventHandler(EventHandler const&);// Don't Implement
		void operator=(EventHandler const&);// Don't implement
		typedef std::map<GLFWwindow*, EventListener*> InputWindowMap;
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