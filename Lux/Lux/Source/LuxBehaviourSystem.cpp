#include "LuxPCH.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxBehaviourComponent.h"
#include "LuxFreeLookCamera.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxBehaviourSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxRenderWindow.h"
#include "LuxBehaviourSystem.h"

Lux::Core::BehaviourSystem::BehaviourSystem()
{

}

Lux::Core::BehaviourSystem::~BehaviourSystem()
{

}

void Lux::Core::BehaviourSystem::ProcessUpdate(const float a_DeltaTime)
{
	BehaviourMap::iterator it;

	for (it = m_BehaviourMap.begin(); it != m_BehaviourMap.end(); ++it)
	{
		BehaviourComponent* behaviour = it->second->GetRawPtr();
		
		if (behaviour->IsEnabled())
		{
			behaviour->Update(a_DeltaTime);
		}
	}
}

void Lux::Core::BehaviourSystem::AddComponent(void* a_Component, const Key& a_CompType, ObjectHandle<Entity>& a_Entity)
{
	Core::ObjectHandle<Core::Component>* comp = (Core::ObjectHandle<Core::Component>*)a_Component;
	m_BehaviourMap[&a_Entity] = (Core::ObjectHandle<BehaviourComponent>*)(comp);
	BehaviourComponent::InitOptions initOptions;
	initOptions.m_EventListener = m_SceneManager->GetRenderWindow()->GetEventListener();
	initOptions.m_Transform = &m_SceneManager->GetComponent<Transform>(a_Entity);

	m_BehaviourMap[&a_Entity]->GetRawPtr()->Init(initOptions);
	m_BehaviourMap[&a_Entity]->GetRawPtr()->Start();
}

void Lux::Core::BehaviourSystem::RemoveComponent(const Key& a_CompType, ObjectHandle<Entity>& a_Entity)
{
	m_BehaviourMap[&a_Entity]->GetRawPtr()->OnDestroy();
	m_BehaviourMap.erase(&a_Entity);
}

bool Lux::Core::BehaviourSystem::Init(SceneManager* a_SceneManager)
{
	System::Init(a_SceneManager);
	return true;
}
