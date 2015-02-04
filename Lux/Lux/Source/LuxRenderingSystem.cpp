#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxMaterial.h"
#include "LuxMaterialComponent.h"
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
#include "LuxShader.h"
#include "LuxCamera.h"
#include "LuxRenderingSystem.h"
#include "LuxTextureSampler.h"
#include "LuxTexture.h"
#include "LuxTexture1D.h"
#include "LuxTexture2D.h"
#include "LuxTexture3D.h"

#define CONVERT_ID_TO_CLASS_STRING(a) "class " ID_TO_STRING(a)
#define ADD_COMPONENT_MAP_INSERT(a, b) m_AddComponentFuncMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))
#define REMOVE_COMPONENT_MAP_INSERT(a, b) m_RemoveComponentProcessMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1)))
Lux::Graphics::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr), m_MainCamera(nullptr), m_MainCameraTransform(nullptr), m_LightEntry(nullptr), m_UniformBuffer(5),
m_MeshRendererKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::MeshRenderer)),
m_TransformKey(CONVERT_ID_TO_CLASS_STRING(Lux::Core::Transform)),
m_ShaderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::ShaderComponent)),
m_CameraKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Camera)),
m_LightKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Light)),
m_MaterialKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::MaterialComponent))
{
	ADD_COMPONENT_MAP_INSERT(m_MeshRendererKey, RenderingSystem::AddComponentInternal<MeshRenderer>);
	ADD_COMPONENT_MAP_INSERT(m_TransformKey, RenderingSystem::AddComponentInternal<Core::Transform>);
	ADD_COMPONENT_MAP_INSERT(m_ShaderKey, RenderingSystem::AddComponentInternal<ShaderComponent>);
	ADD_COMPONENT_MAP_INSERT(m_CameraKey, RenderingSystem::AddComponentInternal<Camera>);
	ADD_COMPONENT_MAP_INSERT(m_LightKey, RenderingSystem::AddComponentInternal<Light>);
	ADD_COMPONENT_MAP_INSERT(m_MaterialKey, RenderingSystem::AddComponentInternal<MaterialComponent>);

	REMOVE_COMPONENT_MAP_INSERT(m_MeshRendererKey, RenderingSystem::RemoveComponentInternal<MeshRenderer>);
	REMOVE_COMPONENT_MAP_INSERT(m_TransformKey, RenderingSystem::RemoveComponentInternal<Core::Transform>);
	REMOVE_COMPONENT_MAP_INSERT(m_ShaderKey, RenderingSystem::RemoveComponentInternal<ShaderComponent>);
	REMOVE_COMPONENT_MAP_INSERT(m_CameraKey, RenderingSystem::RemoveComponentInternal<Camera>);
	REMOVE_COMPONENT_MAP_INSERT(m_LightKey, RenderingSystem::RemoveComponentInternal<Light>);
	REMOVE_COMPONENT_MAP_INSERT(m_MaterialKey, RenderingSystem::RemoveComponentInternal<MaterialComponent>);
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

void Lux::Graphics::RenderingSystem::AddComponent(void* a_Component, const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity)
{
	m_AddComponentFuncMap[a_CompType](a_Component, a_Entity);
}

void Lux::Graphics::RenderingSystem::RemoveComponent(const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity)
{
	bool entryExists = EntityEntryExists(a_Entity);

	if (!entryExists)
	{
		return;
	}

	m_RemoveComponentProcessMap[a_CompType](a_Entity);

	if (m_EntityMap[&a_Entity].IsNull())
	{
		m_EntityMap.erase(&a_Entity);
	}
}

bool Lux::Graphics::RenderingSystem::EntityEntryExists(Core::ObjectHandle<Core::Entity>& a_Entity)
{
	int count = m_EntityMap.count(&a_Entity);

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
				if (it->second.m_Camera->GetRawPtr()->IsMainCamera())
				{
					if (it->second.m_Transform)
					{
						m_MainCamera = it->second.m_Camera;
						m_MainCameraTransform = it->second.m_Transform;
						m_EntityMap.erase(it);
						end = false;
						break;
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
		m_MainCamera->GetRawPtr()->ChangeAspect(aspect);
	}
	
	m_MainCameraTransform->GetRawPtr()->ApplyTransform();

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (!it->second.m_Transform)
			continue;

		it->second.m_Transform->GetRawPtr()->ApplyTransform();

		if (!it->second.m_MeshRenderer || !it->second.m_Material)
			continue;

		Core::Shader* shader = it->second.m_Material->GetRawPtr()->GetShader();
		
		if (!shader)
			continue;

		Core::Mesh* mesh = it->second.m_MeshRenderer->GetRawPtr()->GetMesh();
		
		if (!mesh)
			continue;

		mat4x4& transform = it->second.m_Transform->GetRawPtr()->GetMatrix();

		unsigned int numSubMeshes = mesh->GetNumSubMeshes();

		shader->Activate();

		// Set the default uniform buffer
		Core::ShaderVariable worldMatVal(Core::VALUE_MAT4X4, glm::value_ptr(transform), sizeof(mat4));
		Core::ShaderVariable viewMatVal(Core::VALUE_MAT4X4, glm::value_ptr(m_MainCameraTransform->GetRawPtr()->GetInverseTranslationMatrix()), sizeof(mat4));
		Core::ShaderVariable projMatVal(Core::VALUE_MAT4X4, glm::value_ptr(m_MainCamera->GetRawPtr()->GetProjectionMatrix()), sizeof(mat4));
		Core::ShaderVariable lightVal(Core::VALUE_VEC3, glm::value_ptr(m_LightEntry->m_Transform->GetRawPtr()->GetPosition()), sizeof(vec3));
		Core::ShaderVariable lightCol(Core::VALUE_VEC4, glm::value_ptr(m_LightEntry->m_Light->GetRawPtr()->GetColor()), sizeof(vec4));
		m_UniformBuffer.SetVariable(0, viewMatVal);
		m_UniformBuffer.SetVariable(1, projMatVal);
		m_UniformBuffer.SetVariable(2, worldMatVal);
		m_UniformBuffer.SetVariable(3, lightVal);
		m_UniformBuffer.SetVariable(4, lightCol);

		if (!it->second.m_Init)
		{
			mesh->ConnectWithShader(shader);
			shader->InitializeUniformBuffer("UniformBuffer0", m_UniformBuffer, VERTEX_PROGRAM);
			it->second.m_Init = true;
		}

		// Bind Samplers and Textures
		Core::Texture2D* diffuseTex = it->second.m_Material->GetRawPtr()->GetDiffuseTexture();

		if (!diffuseTex)
			continue;

		Core::TextureSampler* texSampler = diffuseTex->GetSampler();
		texSampler->Activate(0, FRAGMENT_PROGRAM); // 0 is the diffuse texture
		diffuseTex->Bind(0, "DiffuseTexture", shader, FRAGMENT_PROGRAM);

		shader->Update();

		for (unsigned int i = 0; i < numSubMeshes; ++i)
		{
			Core::SubMesh* subMesh = mesh->GetSubMesh(i);
			LuxAssert(subMesh);
			subMesh->PreRender();
			m_RenderWindow->Render(subMesh);
			subMesh->PostRender();
		}
		
		diffuseTex->Unbind();
		texSampler->Deactivate();

		shader->Deactivate();
	}
}
