#ifndef LUX_PLATFORM_OGL_H
#define LUX_PLATFORM_OGL_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class RenderWindowOGL;
			class PlatformOGL : public Platform
			{
			public:
				virtual ~PlatformOGL();
				virtual bool Init(Utility::AppInitOptions& a_AppInitOptions);
				virtual RenderWindow* GetRenderWindow();
				virtual const bool ShouldQuit();
			protected:
				PlatformOGL();
				friend class Platform;
				RenderWindowOGL* m_Window;
			};
		}
	}
}

#endif