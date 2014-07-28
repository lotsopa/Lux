#include "LuxPCH.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"

Lux::SceneManager::SceneManager() : m_NumComponentTypes(0)
{
	m_ComponentFactory = new ComponentFactory();
	m_EntityFactory = new EntityFactory(LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE, LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT);
}

Lux::SceneManager::~SceneManager()
{
	m_ComponentIndexMap.clear();
	m_EntityComponentMap.clear();
	m_DelFuncMap.clear();
	SafePtrDelete(m_ComponentFactory);
	SafePtrDelete(m_EntityFactory);
}

Lux::Entity* Lux::SceneManager::CreateEntity()
{
	Entity* ent = m_EntityFactory->CreateEntity();
	m_EntityComponentMap.insert(std::make_pair(ent, SceneManager::ComponentLayout(m_NumComponentTypes, this)));
	return ent;
}

bool Lux::SceneManager::DestroyEntity(Entity* a_Ent)
{
	m_EntityComponentMap.erase(a_Ent);
	bool ret = m_EntityFactory->DestroyEntity(a_Ent);
	return ret;
}
