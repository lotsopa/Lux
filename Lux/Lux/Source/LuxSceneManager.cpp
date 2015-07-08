#include "LuxPCH.h"
#include "LuxKeyCodes.h"
#include "LuxObjectPool.h"
#include "LuxEntity.h"
#include "LuxComponent.h"
#include "LuxTransform.h"
#include "LuxMaterial.h"
#include "LuxMaterial.h"
#include "LuxKey.h"
#include "LuxModel.h"
#include "LuxLight.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxShader.h"
#include "LuxRenderingSystem.h"
#include "LuxBehaviourSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxBehaviour.h"
#include "LuxFreeLookCamera.h"

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
	m_CompDepCreateMap.clear();
	m_ComponentDependencyMap.clear();
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

 Lux::Core::ObjectHandle<Lux::Core::Entity>& Lux::Core::SceneManager::CreateEntity()
{
	 ObjectHandle<Entity>& ent = m_EntityFactory->CreateEntity();
	m_EntityComponentMap.insert(std::make_pair(&ent, ComponentLayout(m_NumComponentTypes, ent, this)));
	return ent;
}

 bool Lux::Core::SceneManager::DestroyEntity(ObjectHandle<Lux::Core::Entity>& a_Ent)
{
	m_EntityComponentMap.erase(&a_Ent);
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

void Lux::Core::SceneManager::AddDependenciesRecursive(const Key& a_CurrCompKey, const Key& a_CheckKey, unsigned int a_Depth)
{
	if (a_Depth > LUX_COMPONENT_DEPENDENCIES_MAX_RECURSION)
	{
		Utility::ThrowError("Component Dependency Max Recursion Reached.");
	}

	bool keyExists = m_ComponentDependencyMap.count(a_CheckKey) > 0;
	if (!keyExists)
		return;

	ComponentDependencyMap::iterator iter1 = m_ComponentDependencyMap.lower_bound(a_CheckKey);
	ComponentDependencyMap::iterator iter2 = m_ComponentDependencyMap.upper_bound(a_CheckKey);

	while (iter1 != iter2)
	{
		// Make sure we don't add the same dependency twice
		if (ComponentDependencyExists(a_CurrCompKey, iter1->second))
		{
			++iter1;
			continue;
		}

		AddDependenciesRecursive(a_CurrCompKey, iter1->second, a_Depth++);

		// Make sure we don't add the same dependency twice
		if (ComponentDependencyExists(a_CurrCompKey, iter1->second))
		{
			++iter1;
			continue;
		}
		m_ComponentDependencyMap.insert(std::make_pair(a_CurrCompKey, iter1->second));
		++iter1;
	}
}

bool Lux::Core::SceneManager::ComponentDependencyExists(const Key& a_Key, const Key& a_DepKey)
{
	ComponentDependencyMap::iterator it1 = m_ComponentDependencyMap.lower_bound(a_Key);
	ComponentDependencyMap::iterator it2 = m_ComponentDependencyMap.upper_bound(a_Key);

	while (it1 != it2)
	{
		if (it1->second == a_DepKey)
			return true; // Already added, just return

		++it1;
	}
	return false;
}

Lux::Core::ComponentLayout& Lux::Core::SceneManager::GetComponentLayout(ObjectHandle<Entity>& a_Entity)
{
	return m_EntityComponentMap.at(&a_Entity);
}

Lux::Core::ComponentLayout::ComponentEntry::~ComponentEntry()
{
	m_Data = nullptr;
}

Lux::Core::ComponentLayout::ComponentEntry::ComponentEntry() : m_Data(nullptr)
{

}

Lux::Core::ComponentLayout::~ComponentLayout()
{
	SceneManager::ComponentDelFuncMap::iterator it;

	unsigned int ctr = 0;
	for (it = m_SceneManager->m_DelFuncMap.begin(); it != m_SceneManager->m_DelFuncMap.end(); ++it)
	{
		if (m_Components[it->first].m_Data != nullptr)
		{
			it->second(m_Components[it->first].m_Data, m_Owner);
		}
	}

	Utility::SafeArrayDelete(m_Components);
}

Lux::Core::ComponentLayout::ComponentLayout(const ComponentLayout& a_Layout) : m_Owner(a_Layout.m_Owner)
{
	m_NumComponents = a_Layout.m_NumComponents;
	m_SceneManager = a_Layout.m_SceneManager;
	m_Components = new ComponentEntry[m_NumComponents];
	for (unsigned int i = 0; i < m_NumComponents; i++)
	{
		m_Components[i].m_Data = a_Layout.m_Components->m_Data;
	}
}

Lux::Core::ComponentLayout::ComponentLayout(const unsigned int a_NumComponents, ObjectHandle<Entity>& a_Owner, SceneManager* a_Manager) :
m_NumComponents(a_NumComponents), m_Owner(a_Owner), m_SceneManager(a_Manager)
{
	m_Components = new ComponentEntry[m_NumComponents];
}
