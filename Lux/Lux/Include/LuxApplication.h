#ifndef LUX_APPLICATION_H
#define LUX_APPLICATION_H

namespace Lux
{
	class RenderWindow;
	class Application
	{
	public:
		Application();
		~Application();

		bool Initialize(unsigned int a_Width, unsigned int a_Height, String a_Caption, unsigned int a_GLVerMajor = 3, unsigned int a_GLVerMinor = 3, unsigned int a_AA = 0, TLogLevel a_LogLevel = logDEBUG4);
		void Terminate();

		virtual bool LoadContent();
		virtual bool UnloadContent();

		virtual void Render();
		virtual void Update();

		const virtual bool ShouldQuit();

	private:

	protected:
		RenderWindow* m_Window;

	};
}

#endif