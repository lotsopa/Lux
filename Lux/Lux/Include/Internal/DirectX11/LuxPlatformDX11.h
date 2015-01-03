#ifndef LUX_PLATFORM_DX11_H
#define LUX_PLATFORM_DX11_H

namespace Lux
{
	namespace Core
	{
		namespace Internal
		{
			class RenderWindowDX11;

			class PlatformDX11 : public Platform
			{
			public:
				virtual ~PlatformDX11();
				virtual bool Init(Utility::AppInitOptions& a_AppInitOptions);
				virtual RenderWindow* GetRenderWindow();
				virtual const bool ShouldQuit();

			protected:
				PlatformDX11();
				RenderWindowDX11* m_RenderWindow;
				friend class Platform;
			};
		}
	}
}

#endif