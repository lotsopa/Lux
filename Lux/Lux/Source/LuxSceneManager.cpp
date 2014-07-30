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
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"

Lux::SceneManager::SceneManager(RenderWindow* a_RenderWindow) : 
m_NumComponentTypes(0),
m_RenderWindow(a_RenderWindow)
{
	m_ComponentFactory = new ComponentFactory();
	m_EntityFactory = new EntityFactory(LUX_ENTITY_MEMORY_POOL_INITIAL_SIZE, LUX_ENTITY_MEMORY_POOL_GROW_AMOUNT);
	m_SystemFactory = new SystemFactory();
}

Lux::SceneManager::~SceneManager()
{
	m_ComponentIndexMap.clear();
	m_EntityComponentMap.clear();
	m_DelFuncMap.clear();
	SafePtrDelete(m_ComponentFactory);
	SafePtrDelete(m_EntityFactory);
	SafePtrDelete(m_SystemFactory);

	SystemsVector::iterator sysIt;

	for (sysIt = m_SystemsVector.begin(); sysIt != m_SystemsVector.end(); ++sysIt)
	{
		delete (*sysIt);
	}
	m_SystemsVector.clear();
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

void Lux::SceneManager::ProcessUpdate(const float a_Dt)
{
	ProcessSystems(a_Dt);
}

void Lux::SceneManager::ProcessSystems(const float a_DeltaTime)
{
	SystemsVector::iterator sysIt;

	for (sysIt = m_SystemsVector.begin(); sysIt != m_SystemsVector.end(); ++sysIt)
	{
		System* systemPtr = (*sysIt);
		systemPtr->ProcessUpdate(a_DeltaTime);
	}
}
