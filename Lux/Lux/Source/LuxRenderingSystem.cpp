#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxSystemMessage.h"
#include "LuxMeshRenderer.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"

Lux::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr)
{

}

Lux::RenderingSystem::~RenderingSystem()
{

}

void Lux::RenderingSystem::ProcessUpdate(const float a_DeltaTime)
{
	// remove and insert the queued entries
	InsertEntries();
	RemoveEntries();

	glfwSwapBuffers(m_RenderWindow->GetWindowHandle());
}

bool Lux::RenderingSystem::Init(SceneManager* a_SceneManager)
{
	System::Init(a_SceneManager);
	m_RenderWindow = m_SceneManager->GetRenderWindow();
	return true;
}

void Lux::RenderingSystem::AddComponent(Component* a_Comp, Entity* a_Entity)
{
	// TODO - Figure out a better/faster way to deduce component types
	Transform* transformPtr = nullptr;
	MeshRenderer* meshPtr = nullptr;

	transformPtr = dynamic_cast<Transform*>(a_Comp);

	if (transformPtr == nullptr)
	{
		// Must be a mesh pointer
		meshPtr = dynamic_cast<MeshRenderer*>(a_Comp);

		if (meshPtr == nullptr)
		{
			ThrowError("Could not deduce the Component type passed to the Rendering System.");
		}

		// We are sure this is a Mesh Renderer component
		//m_EntityMap[a_Entity].m_MeshRenderer = meshPtr;
		m_MeshRendererInsertQueue.Push(std::make_pair(a_Entity, meshPtr));
	}
	else
	{
		// We are sure this is a Transform component
		//m_EntityMap[a_Entity].m_Transform = transformPtr;
		m_TransformInsertQueue.Push(std::make_pair(a_Entity, transformPtr));
	}
}

void Lux::RenderingSystem::RemoveComponent(Component* a_Component, Entity* a_Entity)
{
	bool entryExists = EntityEntryExists(a_Entity);

	if (!entryExists)
	{
		return;
	}

	Transform* transformPtr = nullptr;
	MeshRenderer* meshPtr = nullptr;

	transformPtr = dynamic_cast<Transform*>(a_Component);

	if (transformPtr == nullptr)
	{
		// Must be a mesh pointer
		meshPtr = dynamic_cast<MeshRenderer*>(a_Component);

		if (meshPtr == nullptr)
		{
			ThrowError("Could not deduce the Component type passed to the Rendering System.");
		}

		// We are sure this is a Mesh Renderer component
		//m_EntityMap[a_Entity].m_MeshRenderer = nullptr;
		m_MeshRendererRemoveQueue.Push(a_Entity);
	}
	else
	{
		// We are sure this is a Transform component
		//m_EntityMap[a_Entity].m_Transform = nullptr;
		m_TransformRemoveQueue.Push(a_Entity);
	}
}

bool Lux::RenderingSystem::EntityEntryExists(Entity* a_Entity)
{
	int count = m_EntityMap.count(a_Entity);

	if (count > 0)
	{
		return true;
	}

	return false;
}

void Lux::RenderingSystem::InsertEntries()
{
	while (!m_TransformInsertQueue.Empty())
	{
		std::pair<Entity*, Transform*> p = m_TransformInsertQueue.Pop();
		m_EntityMap[p.first].m_Transform = p.second;
	}

	while (!m_MeshRendererInsertQueue.Empty())
	{
		std::pair<Entity*, MeshRenderer*> p = m_MeshRendererInsertQueue.Pop();
		m_EntityMap[p.first].m_MeshRenderer = p.second;
	}
}

void Lux::RenderingSystem::RemoveEntries()
{
	while (!m_TransformInsertQueue.Empty())
	{
		Entity* e = m_TransformRemoveQueue.Pop();
		m_EntityMap[e].m_Transform = nullptr;
	}

	while (!m_MeshRendererInsertQueue.Empty())
	{
		Entity* e = m_MeshRendererRemoveQueue.Pop();
		m_EntityMap[e].m_MeshRenderer = nullptr;
	}
}
