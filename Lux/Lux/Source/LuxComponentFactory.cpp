#include "LuxPCH.h"
#include "LuxKey.h"
#include "LuxModel.h"
#include "LuxTransform.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"

Lux::Core::ComponentFactory::ComponentFactory()
{

}

Lux::Core::ComponentFactory::~ComponentFactory()
{
	ComponentDelFuncMap::iterator it;

	for (it = m_ComponentDeleteMap.begin(); it != m_ComponentDeleteMap.end(); ++it)
	{
		it->second();
	}
	m_ComponentDeleteMap.clear();
	m_ComponentPoolMap.clear();
}
