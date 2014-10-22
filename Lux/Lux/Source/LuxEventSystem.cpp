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
#include "LuxEventHandler.h"
#include "LuxEventListener.h"

Lux::Core::EventSystem::EventSystem() : 
System(), m_RenderWindow(nullptr)
{

}

Lux::Core::EventSystem::~EventSystem()
{
	EventHandler::GetInstance().UnregisterEventSystem(this);
}

void Lux::Core::EventSystem::ProcessUpdate(const float a_DeltaTime)
{
	glfwPollEvents();
}

bool Lux::Core::EventSystem::Init(Core::SceneManager* a_SceneManager)
{
	Core::System::Init(a_SceneManager);
	m_RenderWindow = m_SceneManager->GetRenderWindow();
	EventHandler::GetInstance().RegisterEventSystem(this);
	return true;
}

void Lux::Core::EventSystem::AddComponent(Core::Component* a_Component, Core::Entity* a_Entity)
{
	m_EntityMap[a_Entity] = (EventListener*)a_Component;
}

void Lux::Core::EventSystem::RemoveComponent(Core::Component* a_Component, Core::Entity* a_Entity)
{
	m_EntityMap.erase(a_Entity);
}

void Lux::Core::EventSystem::OnKeyDown(int a_Key, int a_ScanCode)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnKeyDown(a_Key, a_ScanCode);
	}
}

void Lux::Core::EventSystem::OnKeyUp(int a_Key, int a_ScanCode)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnKeyUp(a_Key, a_ScanCode);
	}
}

void Lux::Core::EventSystem::OnMouseButtonDown(int a_Btn)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMouseButtonDown(a_Btn);
	}
}

void Lux::Core::EventSystem::OnMouseButtonUp(int a_Btn)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMouseButtonUp(a_Btn);
	}
}

void Lux::Core::EventSystem::OnMouseScroll(double a_OffsetX, double a_OffsetY)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMouseScroll(a_OffsetX, a_OffsetY);
	}
}

void Lux::Core::EventSystem::OnMouseEnterWindow()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMouseEnterWindow();
	}
}

void Lux::Core::EventSystem::OnMouseLeaveWindow()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMouseLeaveWindow();
	}
}

void Lux::Core::EventSystem::OnMousePositionChanged(double a_X, double a_Y)
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		it->second->OnMousePositionChanged(a_X, a_Y);
	}
}
