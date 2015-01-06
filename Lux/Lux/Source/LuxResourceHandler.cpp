#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxKeyCodes.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerOGL.h"
#include "LuxRenderWindow.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include "LuxResourceHandlerDX11.h"
#include "LuxRenderWindowDX11.h"
#endif

Lux::Core::ResourceHandler::ResourceHandler()
{

}

Lux::Core::ResourceHandler::~ResourceHandler()
{

}

Lux::Core::ResourceHandler* Lux::Core::ResourceHandler::Create(Utility::PlatformType a_PlatformType, RenderWindow* a_RenderWindow)
{
	ResourceHandler* retVal = nullptr;
	switch (a_PlatformType)
	{
	case Utility::OPEN_GL:
		retVal = new Internal::ResourceHandlerOGL();
		break;

	case Utility::DIRECTX_11:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
		Internal::RenderWindowDX11* dxWindow = (Internal::RenderWindowDX11*)a_RenderWindow;
		retVal = new Internal::ResourceHandlerDX11(dxWindow);
#else
		Utility::ThrowError("DirectX is not supported on this machine!");
#endif
		break;
	}

	return retVal;
}
