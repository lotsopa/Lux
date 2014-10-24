#include "LuxPCH.h"
#include "LuxHelpers.h"
#include "LuxResourceHandler.h"
#include "LuxResourceHandlerOGL.h"

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
		Utility::ThrowError("Resource Handler Not implemented For this Platform.");
		break;
	}

	return retVal;
}
