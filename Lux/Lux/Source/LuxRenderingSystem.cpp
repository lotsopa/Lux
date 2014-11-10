#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxLight.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxMeshRenderer.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxRenderingSystem.h"
#include "LuxShader.h"
#include "LuxShaderComponent.h"
#include "LuxCamera.h"

#define CONVERT_ID_TO_CLASS_STRING(a) "class " ID_TO_STRING(a)

Lux::Graphics::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr), m_MainCamera(nullptr), m_MainCameraTransform(nullptr), m_LightEntry(nullptr),
m_MeshRendererKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::MeshRenderer)),
m_TransformKey(CONVERT_ID_TO_CLASS_STRING(Lux::Core::Transform)),
m_ShaderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::ShaderComponent)),
m_CameraKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Camera)),
m_LightKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Light))
{
	
}

Lux::Graphics::RenderingSystem::~RenderingSystem()
{

}

void Lux::Graphics::RenderingSystem::ProcessUpdate(const float a_DeltaTime)
{
	m_RenderWindow->Clear();
	RenderPass();
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

		if (m_EntityMap[a_Entity].m_Shader)
		{
			m_EntityMap[a_Entity].m_Init = false;
		}
	}
	else if (a_CompType == m_ShaderKey)
	{
		m_EntityMap[a_Entity].m_Shader = static_cast<Lux::Graphics::ShaderComponent*>(a_Comp);

		if (m_EntityMap[a_Entity].m_MeshRenderer)
		{
			m_EntityMap[a_Entity].m_Init = false;
		}
	}
	else if (a_CompType == m_CameraKey)
	{
		m_EntityMap[a_Entity].m_Camera = static_cast<Lux::Graphics::Camera*>(a_Comp);

		if (m_EntityMap[a_Entity].m_Camera->IsMainCamera())
		{
			m_MainCamera = m_EntityMap[a_Entity].m_Camera;
		}
	}
	else if (a_CompType == m_LightKey)
	{
		m_EntityMap[a_Entity].m_Light = static_cast<Lux::Graphics::Light*>(a_Comp);
		m_LightEntry = &m_EntityMap[a_Entity];
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
	else if (a_CompType == m_CameraKey)
	{
		if (m_EntityMap[a_Entity].m_Camera->IsMainCamera())
			m_MainCamera = nullptr;

		m_EntityMap[a_Entity].m_Camera = nullptr;
	}
	else if (a_CompType == m_LightKey)
	{
		m_EntityMap[a_Entity].m_Light = nullptr;
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
	if (!m_LightEntry)
		return;

	EntityMap::iterator it;
	if (!m_MainCamera)
	{
		bool end = true;
		for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
		{
			if (it->second.m_Camera)
			{
				if (it->second.m_Camera->IsMainCamera())
				{
					if (it->second.m_Transform)
					{
						m_MainCamera = it->second.m_Camera;
						m_MainCameraTransform = it->second.m_Transform;
						end = false;
					}
				}
			}
		}

		if (end)
			return;
	}

	// Resize Window if needed
	if (m_RenderWindow->IsWindowResized())
	{
		float aspect = m_RenderWindow->GetWidth() / (float)m_RenderWindow->GetHeight();
		m_MainCamera->ChangeAspect(aspect);
	}

	m_MainCameraTransform->ApplyTransform();
	const mat4x4 ViewProjMatrix = m_MainCamera->GetProjectionMatrix() * m_MainCameraTransform->GetMatrix();

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (!it->second.m_Transform)
			continue;

		it->second.m_Transform->ApplyTransform();

		if (!it->second.m_MeshRenderer || !it->second.m_Shader)
			continue;

		Core::Shader* shader = it->second.m_Shader->GetShader();
		
		if (!shader)
			continue;

		Core::Mesh* mesh = it->second.m_MeshRenderer->GetMesh();
		
		if (!mesh)
			continue;

		const mat4x4& transform = it->second.m_Transform->GetMatrix();
		mat4x4 modelViewProjMat = ViewProjMatrix * transform;
		unsigned int numSubMeshes = mesh->GetNumSubMeshes();

		shader->Activate();

		if (!it->second.m_Init)
		{
			mesh->ConnectWithShader(shader);
			it->second.m_Init = true;
		}

		shader->SetUniformMat4x4("MVP", modelViewProjMat);
		shader->SetUniformVec3("LightPos", m_LightEntry->m_Transform->GetPosition());
		shader->SetUniformVec4("LightColor", m_LightEntry->m_Light->GetColor());
		for (unsigned int i = 0; i < numSubMeshes; ++i)
		{
			Core::SubMesh* subMesh = mesh->GetSubMesh(i);
			LuxAssert(subMesh);
			subMesh->PreRender();
			m_RenderWindow->Render(subMesh);
			subMesh->PostRender();
		}
		
		shader->Deactivate();
	}
}
