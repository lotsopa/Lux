#include "LuxPCH.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"

Lux::Core::SceneManager::SceneManager(RenderWindow* a_RenderWindow) : 
m_NumComponentTypes(0),
m_RenderWindow(a_RenderWindow)
{
	m_ComponentFactory = new ComponentFactory();
	m_EntityFactory = new EntityFactory(LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE, LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT);
	m_SystemFactory = new SystemFactory();
}

Lux::Core::SceneManager::~SceneManager()
{
	m_ComponentIndexMap.clear();
	m_EntityComponentMap.clear();
	m_DelFuncMap.clear();
	Utility::SafePtrDelete(m_ComponentFactory);
	Utility::SafePtrDelete(m_EntityFactory);
	Utility::SafePtrDelete(m_SystemFactory);

	SystemsMap::iterator sysIt;

	for (sysIt = m_SystemsMap.begin(); sysIt != m_SystemsMap.end(); ++sysIt)
	{
		delete sysIt->second;
	}
	m_SystemsMap.clear();
}

Lux::Core::Entity* Lux::Core::SceneManager::CreateEntity()
{
	Entity* ent = m_EntityFactory->CreateEntity();
	m_EntityComponentMap.insert(std::make_pair(ent, SceneManager::ComponentLayout(m_NumComponentTypes, ent, this)));
	return ent;
}

bool Lux::Core::SceneManager::DestroyEntity(Entity* a_Ent)
{
	m_EntityComponentMap.erase(a_Ent);
	bool ret = m_EntityFactory->DestroyEntity(a_Ent);
	return ret;
}

void Lux::Core::SceneManager::ProcessUpdate(const float a_Dt)
{
	ProcessSystems(a_Dt);
}

void Lux::Core::SceneManager::ProcessSystems(const float a_DeltaTime)
{
	SystemsMap::iterator sysIt;

	for (sysIt = m_SystemsMap.begin(); sysIt != m_SystemsMap.end(); ++sysIt)
	{
		System* systemPtr = sysIt->second;
		systemPtr->ProcessUpdate(a_DeltaTime);
	}
}
