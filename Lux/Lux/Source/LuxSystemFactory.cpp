#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxKey.h"
#include "LuxSystem.h"
#include "LuxShader.h"
#include "LuxRenderingSystem.h"
#include "LuxSystemFactory.h"

Lux::Core::SystemFactory::SystemFactory()
{

}

Lux::Core::SystemFactory::~SystemFactory()
{
	m_SystemTypeMap.clear();
}
