#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxSystemFactory.h"

Lux::SystemFactory::SystemFactory()
{

}

Lux::SystemFactory::~SystemFactory()
{
	m_SystemTypeMap.clear();
}
