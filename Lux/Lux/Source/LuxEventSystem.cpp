#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxKey.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxEventSystem.h"
#include "LuxEventListener.h"

Lux::Core::EventSystem::EventSystem() : 
System()
{

}

Lux::Core::EventSystem::~EventSystem()
{
	
}

void Lux::Core::EventSystem::ProcessUpdate(const float a_DeltaTime)
{
	m_RenderWindow->PollEvents();
}

bool Lux::Core::EventSystem::Init(Core::SceneManager* a_SceneManager)
{
	Core::System::Init(a_SceneManager);
	m_RenderWindow = m_SceneManager->GetRenderWindow();
	return true;
}

void Lux::Core::EventSystem::AddComponent(Component* a_Component, const Key& a_CompType, Entity* a_Entity)
{

}

void Lux::Core::EventSystem::RemoveComponent(const Key& a_CompType, Entity* a_Entity)
{

}
