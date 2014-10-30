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
#include "LuxShaderComponent.h"

#define CONVERT_ID_TO_CLASS_STRING(a) "class " ID_TO_STRING(a)

Lux::Graphics::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr), 
m_MeshRendererKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::MeshRenderer)),
m_TransformKey(CONVERT_ID_TO_CLASS_STRING(Lux::Core::Transform)),
m_ShaderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::ShaderComponent))
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

void Lux::Graphics::RenderingSystem::AddComponent(Core::Component* a_Comp, const Core::Key& a_CompType, Core::Entity* a_Entity)
{
	if (a_CompType == m_TransformKey)
	{
		m_EntityMap[a_Entity].m_Transform = static_cast<Lux::Core::Transform*>(a_Comp);
	}
	else if (a_CompType == m_MeshRendererKey)
	{
		m_EntityMap[a_Entity].m_MeshRenderer = static_cast<Lux::Graphics::MeshRenderer*>(a_Comp);
	}
	else if (a_CompType == m_ShaderKey)
	{
		m_EntityMap[a_Entity].m_Shader = static_cast<Lux::Graphics::ShaderComponent*>(a_Comp);
	}
}

void Lux::Graphics::RenderingSystem::RemoveComponent(const Core::Key& a_CompType, Core::Entity* a_Entity)
{
	bool entryExists = EntityEntryExists(a_Entity);

	if (!entryExists)
	{
		return;
	}

	if (a_CompType == m_TransformKey)
	{
		m_EntityMap[a_Entity].m_Transform = nullptr;
	}
	else if (a_CompType == m_MeshRendererKey)
	{
		m_EntityMap[a_Entity].m_MeshRenderer = nullptr;
	}
	else if (a_CompType == m_ShaderKey)
	{
		m_EntityMap[a_Entity].m_Shader = nullptr;
	}


	if (m_EntityMap[a_Entity].IsNull())
	{
		m_EntityMap.erase(a_Entity);
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

void Lux::Graphics::RenderingSystem::RenderPass()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (!it->second.m_MeshRenderer || !it->second.m_Shader || !it->second.m_Transform)
			continue;

		// TODO Actual rendering
	}
}
