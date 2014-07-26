#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxObjectPool.h"
#include "LuxMesh.h"
#include "LuxTransform.h"
#include "LuxComponentFactory.h"

Lux::ComponentFactory::ComponentFactory()
{

}

Lux::ComponentFactory::~ComponentFactory()
{
	ComponentDelFuncMap::iterator it;

	for (it = m_ComponentDeleteMap.begin(); it != m_ComponentDeleteMap.end(); ++it)
	{
		it->second();
	}
	m_ComponentDeleteMap.clear();
	m_ComponentPoolMap.clear();
}
