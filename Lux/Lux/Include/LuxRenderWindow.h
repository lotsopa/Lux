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
			virtual ~RenderWindow();

			virtual bool Initialize(Utility::AppInitOptions& a_AppInitOptions) = 0;

			virtual EventListener* GetEventListener() = 0;

			virtual void SwapBuffers() = 0;

		protected:
			RenderWindow();

		};
	}
}

#endif