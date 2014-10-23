#ifndef LUX_PLATFORM_H
#define LUX_PLATFORM_H

namespace Lux
{
	namespace Core
	{
		class RenderWindow;
		class Platform
		{
		public:
			static Platform* Create(Utility::PlatformType a_PlatformType);
			virtual ~Platform();

			virtual bool Init(Utility::AppInitOptions& a_AppInitOptions) = 0;
			virtual RenderWindow* GetRenderWindow() = 0;
			virtual const bool ShouldQuit() = 0;

		protected:
			Platform();
			Platform(Platform const&);

		};
	}
}

#endif