#ifndef LUX_APPLICATION_H
#define LUX_APPLICATION_H

namespace Lux
{
	namespace Core
	{
		class RenderWindow;
		class SceneManager;
		class Platform;
		class ResourceHandler;
	}

	class Application
	{
	public:
		Application();
		virtual ~Application();

		bool Initialize(Utility::AppInitOptions& a_AppInitOptions);
		void Run();
		void Terminate();

		virtual bool LoadComponentTypes();
		virtual bool LoadSystemTypes();
		virtual bool LoadContent() = 0;
		virtual bool UnloadContent() = 0;

		virtual bool Update(const float a_DeltaTime) = 0; // Not sure about this one
		//virtual bool OnFrameStarted(const float a_DeltaTime) = 0;
		//virtual bool OnFrameEnded(const float a_DeltaTime) = 0;

		inline double GetTimeSinceStart() { return glfwGetTime();  }

		const virtual bool ShouldQuit();

	private:
		void InternalUpdate(const float a_DeltaTime);
		void CheckResult(bool res);

	protected:
		Core::Platform* m_Platform;
		Core::SceneManager* m_SceneManager;
		Core::ResourceHandler* m_ResourceHandler;
	};
}

#endif