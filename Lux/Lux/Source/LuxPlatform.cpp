#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxRenderWindow.h"
#include "LuxPlatform.h"
#include "LuxPlatformOGL.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include "LuxPlatformDX11.h"
#endif

Lux::Core::Platform::Platform()
{

}

Lux::Core::Platform::~Platform()
{

}

Lux::Core::Platform* Lux::Core::Platform::Create(Utility::PlatformType a_PlatformType)
{
	Platform* retVal = nullptr;
	switch (a_PlatformType)
	{
	case Utility::OPEN_GL:
		retVal = new Internal::PlatformOGL();
		break;

	case Utility::DIRECTX_11:
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
		retVal = new Internal::PlatformDX11();

#else
		Utility::ThrowError("DirectX is not supported on this machine!");
#endif
		break;
	}

	return retVal;
}
