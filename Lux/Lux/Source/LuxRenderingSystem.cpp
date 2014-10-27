#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxMeshRenderer.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"

Lux::Graphics::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr)
{

}

Lux::Graphics::RenderingSystem::~RenderingSystem()
{

}

void Lux::Graphics::RenderingSystem::ProcessUpdate(const float a_DeltaTime)
{
	m_RenderWindow->SwapBuffers();
}

bool Lux::Graphics::RenderingSystem::Init(Core::SceneManager* a_SceneManager)
{
	Core::System::Init(a_SceneManager);
	m_RenderWindow = m_SceneManager->GetRenderWindow();
	return true;
}

void Lux::Graphics::RenderingSystem::AddComponent(Core::Component* a_Comp, Core::Entity* a_Entity)
{
	// TODO - Figure out a better/faster way to deduce component types
	Core::Transform* transformPtr = nullptr;
	MeshRenderer* meshPtr = nullptr;

	transformPtr = dynamic_cast<Core::Transform*>(a_Comp);

	if (transformPtr == nullptr)
	{
		// Must be a mesh pointer
		meshPtr = dynamic_cast<MeshRenderer*>(a_Comp);

		if (meshPtr == nullptr)
		{
			Utility::ThrowError("Could not deduce the Component type passed to the Rendering System.");
		}

		// We are sure this is a Mesh Renderer component
		m_EntityMap[a_Entity].m_MeshRenderer = meshPtr;
	}
	else
	{
		// We are sure this is a Transform component
		m_EntityMap[a_Entity].m_Transform = transformPtr;
	}
}

void Lux::Graphics::RenderingSystem::RemoveComponent(Core::Component* a_Component, Core::Entity* a_Entity)
{
	bool entryExists = EntityEntryExists(a_Entity);

	if (!entryExists)
	{
		return;
	}

	Core::Transform* transformPtr = nullptr;
	MeshRenderer* meshPtr = nullptr;

	transformPtr = dynamic_cast<Core::Transform*>(a_Component);

	if (transformPtr == nullptr)
	{
		// Must be a mesh pointer
		meshPtr = dynamic_cast<MeshRenderer*>(a_Component);

		if (meshPtr == nullptr)
		{
			Utility::ThrowError("Could not deduce the Component type passed to the Rendering System.");
		}

		// We are sure this is a Mesh Renderer component
		m_EntityMap[a_Entity].m_MeshRenderer = nullptr;

		if (m_EntityMap[a_Entity].IsNull())
		{
			m_EntityMap.erase(a_Entity);
		}
	}
	else
	{
		// We are sure this is a Transform component
		m_EntityMap[a_Entity].m_Transform = nullptr;

		if (m_EntityMap[a_Entity].IsNull())
		{
			m_EntityMap.erase(a_Entity);
		}
	}
}

bool Lux::Graphics::RenderingSystem::EntityEntryExists(Core::Entity* a_Entity)
{
	int count = m_EntityMap.count(a_Entity);

	if (count > 0)
	{
		return true;
	}

	return false;
}
