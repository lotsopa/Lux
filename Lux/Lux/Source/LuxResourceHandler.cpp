#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerOGL.h"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include "LuxResourceHandlerDX11.h"
#endif

Lux::Core::ResourceHandler::ResourceHandler()
{

}

Lux::Core::ResourceHandler::~ResourceHandler()
{

}

Lux::Core::ResourceHandler* Lux::Core::ResourceHandler::Create(Utility::PlatformType a_PlatformType)
{
	ResourceHandler* retVal = nullptr;
	switch (a_PlatformType)
	{
	case Utility::OPEN_GL:
		retVal = new Internal::ResourceHandlerOGL();
		break;

	case Utility::DIRECTX_11:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
		retVal = new Internal::ResourceHandlerDX11();
#else
		Utility::ThrowError("DirectX is not supported on this machine!");
#endif
		break;
	}

	return retVal;
}
