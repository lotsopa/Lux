#ifndef LUX_EVENT_HANDLER_H
#define LUX_EVENT_HANDLER_H

namespace Lux
{
	namespace Core
	{
		class RenderWindow;
		class EventListener;
		class EventSystem;

		class EventHandler
		{
		public:
			static EventHandler& GetInstance();
			~EventHandler();

			void RegisterEventSystem(EventSystem* a_System);
			void UnregisterEventSystem(EventSystem* a_System);

			bool IsEventSystemRegistered(EventSystem* a_System);

		private:
			EventHandler();
			EventHandler(EventHandler const&);// Don't Implement
			void operator=(EventHandler const&);// Don't implement
			typedef std::map<GLFWwindow*, EventSystem*> InputWindowMap;
			InputWindowMap m_InputWindows;

			// GLFW callbacks
			static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
			static void MouseScrollCallback(GLFWwindow* window, double x, double y);
			static void CursorEnterCallback(GLFWwindow* window, int val);
			static void CursorPosCallback(GLFWwindow* window, double a_X, double a_Y);

			void RelayKeyDown(GLFWwindow* a_Window, int a_Key, int a_ScanCode);
			void RelayKeyUp(GLFWwindow* a_Window, int a_Key, int a_ScanCode);
			void RelayMouseButtonDown(GLFWwindow* a_Window, int a_Btn);
			void RelayMouseButtonUp(GLFWwindow* a_Window, int a_Btn);
			void RelayMouseScroll(GLFWwindow* a_Window, double a_X, double a_Y);
			void RelayMouseEnterWindow(GLFWwindow* a_Window);
			void RelayMouseLeaveWindow(GLFWwindow* a_Window);
			void RelayMousePosition(GLFWwindow* a_Window, double a_X, double a_Y);

			friend class RenderWindow;
		};
	}
}

#endif