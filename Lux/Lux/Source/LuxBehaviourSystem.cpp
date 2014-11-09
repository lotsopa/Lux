#include "LuxPCH.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxBehaviourComponent.h"
#include "LuxFreeLookCamera.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxBehaviourSystem.h"
#include "LuxEventSystem.h"
#include "LuxSystemFactory.h"
#include "LuxSceneManager.h"
#include "LuxRenderWindow.h"
#include "LuxBehaviourComponent.h"
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
		BehaviourComponent* behaviour = it->second;
		
		if (behaviour->IsEnabled())
		{
			behaviour->Update(a_DeltaTime);
		}
	}
}

void Lux::Core::BehaviourSystem::AddComponent(Component* a_Component, const Key& a_CompType, Entity* a_Entity)
{
	m_BehaviourMap[a_Entity] = static_cast<BehaviourComponent*>(a_Component);
	BehaviourComponent::InitOptions initOptions;
	initOptions.m_EventListener = m_SceneManager->GetRenderWindow()->GetEventListener();
	initOptions.m_Transform = m_SceneManager->GetComponent<Transform>(a_Entity);

	m_BehaviourMap[a_Entity]->Init(initOptions);
	m_BehaviourMap[a_Entity]->Start();
}

void Lux::Core::BehaviourSystem::RemoveComponent(const Key& a_CompType, Entity* a_Entity)
{
	m_BehaviourMap[a_Entity]->OnDestroy();
	m_BehaviourMap.erase(a_Entity);
}

bool Lux::Core::BehaviourSystem::Init(SceneManager* a_SceneManager)
{
	System::Init(a_SceneManager);
	return true;
}
