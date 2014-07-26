#ifndef LUX_APPLICATION_H
#define LUX_APPLICATION_H

namespace Lux
{
	class RenderWindow;
	class SceneManager;
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

		virtual bool Update(const float a_DeltaTime) = 0;
		virtual bool OnFrameStarted(const float a_DeltaTime) = 0;
		virtual bool OnFrameEnded(const float a_DeltaTime) = 0;

		inline double GetTimeSinceStart() { return glfwGetTime();  }

		const virtual bool ShouldQuit();

	private:
		void Render(const float a_DeltaTime);
		void PollEvents();
		void CheckResult(bool res);

	protected:
		RenderWindow* m_Window;
		SceneManager* m_SceneManager;
	};
}

#endif