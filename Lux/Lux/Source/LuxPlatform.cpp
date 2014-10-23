#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxRenderWindow.h"
#include "LuxPlatform.h"
#include "LuxPlatformOGL.h"

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
		Utility::ThrowError("Platform type not implemented.");
		break;
	}

	return retVal;
}
