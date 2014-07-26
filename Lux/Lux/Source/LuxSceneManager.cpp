#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxEntity.h"
#include "LuxKey.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"

Lux::SceneManager::SceneManager() : 
m_ComponentFactory(), m_EntityFactory(LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE, LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT)
{

}

Lux::SceneManager::~SceneManager()
{

}
