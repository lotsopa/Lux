#ifndef LUX_APPLICATION_H
#define LUX_APPLICATION_H

namespace Lux
{
	class RenderWindow;
	class Application
	{
	public:
		Application();
		virtual ~Application();

		bool Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_GLVerMajor = 3, unsigned int a_GLVerMinor = 3, unsigned int a_AA = 0, TLogLevel a_LogLevel = logDEBUG4);
		void Run();
		void Terminate();

		virtual bool LoadContent() = 0;
		virtual bool UnloadContent() = 0;

		virtual void Render() = 0;
		virtual void Update(const double a_DeltaTime) = 0;

		void PollEvents();

		inline double GetTimeSinceStart() { return glfwGetTime();  }

		const virtual bool ShouldQuit();

	private:

	protected:
		RenderWindow* m_Window;

	};
}

#endif