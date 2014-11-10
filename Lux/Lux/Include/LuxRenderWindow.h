#ifndef LUX_RENDER_WINDOW_H
#define LUX_RENDER_WINDOW_H

namespace Lux
{
	namespace Core
	{
		class EventListener;
		class SubMesh;

		class RenderWindow
		{
		public:
			virtual ~RenderWindow();

			virtual bool Initialize(Utility::AppInitOptions& a_AppInitOptions) = 0;
			virtual EventListener* GetEventListener() = 0;
			virtual void SwapBuffers() = 0;
			virtual void Clear() = 0;
			virtual void PollEvents() = 0;
			virtual void Render(SubMesh* a_SubMesh) = 0;
			virtual const bool IsWindowResized() = 0;
			virtual const int GetHeight() = 0;
			virtual const int GetWidth() = 0;

		protected:
			RenderWindow();

		};
	}
}

#endif