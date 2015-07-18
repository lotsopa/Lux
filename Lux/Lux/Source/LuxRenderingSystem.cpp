#include "LuxPCH.h"
#include "LuxRenderWindow.h"
#include "LuxComponent.h"
#include "LuxMaterial.h"
#include "LuxMaterial.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxLight.h"
#include "LuxModel.h"
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
#include "LuxMaterialShaderResource.h"
#include "LuxLightShaderResource.h"

#define CONVERT_ID_TO_CLASS_STRING(a) "class " ID_TO_STRING(a)
#define ADD_COMPONENT_MAP_INSERT(a, b) m_AddComponentFuncMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))
#define REMOVE_COMPONENT_MAP_INSERT(a, b) m_RemoveComponentProcessMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1)))
Lux::Graphics::RenderingSystem::RenderingSystem() :
System(), m_RenderWindow(nullptr), m_MainCamera(nullptr), m_MainCameraTransform(nullptr), m_LightEntry(nullptr), m_ObjUniformBuffer(3), m_LightUniformBuffer(2), m_MatUniformBuffer(1),
m_MeshRendererKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::MeshRenderer)),
m_TransformKey(CONVERT_ID_TO_CLASS_STRING(Lux::Core::Transform)),
m_ShaderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::ShaderComponent)),
m_CameraKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Camera)),
m_LightKey(CONVERT_ID_TO_CLASS_STRING(Lux::Graphics::Light))
{
	ADD_COMPONENT_MAP_INSERT(m_MeshRendererKey, RenderingSystem::AddComponentInternal<MeshRenderer>);
	ADD_COMPONENT_MAP_INSERT(m_TransformKey, RenderingSystem::AddComponentInternal<Core::Transform>);
	ADD_COMPONENT_MAP_INSERT(m_ShaderKey, RenderingSystem::AddComponentInternal<ShaderComponent>);
	ADD_COMPONENT_MAP_INSERT(m_CameraKey, RenderingSystem::AddComponentInternal<Camera>);
	ADD_COMPONENT_MAP_INSERT(m_LightKey, RenderingSystem::AddComponentInternal<Light>);

	REMOVE_COMPONENT_MAP_INSERT(m_MeshRendererKey, RenderingSystem::RemoveComponentInternal<MeshRenderer>);
	REMOVE_COMPONENT_MAP_INSERT(m_TransformKey, RenderingSystem::RemoveComponentInternal<Core::Transform>);
	REMOVE_COMPONENT_MAP_INSERT(m_ShaderKey, RenderingSystem::RemoveComponentInternal<ShaderComponent>);
	REMOVE_COMPONENT_MAP_INSERT(m_CameraKey, RenderingSystem::RemoveComponentInternal<Camera>);
	REMOVE_COMPONENT_MAP_INSERT(m_LightKey, RenderingSystem::RemoveComponentInternal<Light>);
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
	Core::MessageManager* msgManager = m_SceneManager->GetMsgManager();
	Core::MessageCallback msgCallback;
	msgCallback.Init<RenderingSystem>(std::bind(&Lux::Graphics::RenderingSystem::MainCameraSet, this, std::placeholders::_1));
	bool callbackRegisterResult = msgManager->RegisterMsgCallback(MSG_MAIN_CAMERA_SET, msgCallback);
	LuxAssert(callbackRegisterResult);
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
	if (!m_LightEntry || !m_MainCamera || !m_MainCameraTransform)
		return;

	// Resize Window if needed
	if (m_RenderWindow->IsWindowResized())
	{
		float aspect = m_RenderWindow->GetWidth() / (float)m_RenderWindow->GetHeight();
		m_MainCamera->GetRawPtr()->ChangeAspect(aspect);
	}
	
	m_MainCameraTransform->GetRawPtr()->ApplyTransform();
	EntityMap::iterator it;
	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (!it->second.m_Transform)
			continue;

		if (!it->second.m_MeshRenderer)
			continue;

		Core::Mesh* mesh = it->second.m_MeshRenderer->GetRawPtr()->GetMesh().get();
		
		if (!mesh)
			continue;

		// Make sure we multiply the transforms properly, taking into account parents
		mat4x4 finalTransform;
		Core::Transform* transformPtr = it->second.m_Transform->GetRawPtr();
		bool hasParent = true;
		do 
		{
			hasParent = transformPtr->GetParentTransform().IsValid();
			transformPtr->ApplyTransform();
			finalTransform *= transformPtr->GetMatrix();
			transformPtr = transformPtr->GetParentTransform().GetRawPtr();

		} while (hasParent);

		// Set the default uniform buffer
		Core::ShaderVariable worldMatVal(Core::VALUE_MAT4X4, glm::value_ptr(finalTransform), sizeof(mat4));
		Core::ShaderVariable viewMatVal(Core::VALUE_MAT4X4, glm::value_ptr(m_MainCameraTransform->GetRawPtr()->GetInverseTranslationMatrix()), sizeof(mat4));
		Core::ShaderVariable projMatVal(Core::VALUE_MAT4X4, glm::value_ptr(m_MainCamera->GetRawPtr()->GetProjectionMatrix()), sizeof(mat4));
		m_ObjUniformBuffer.SetVariable(0, viewMatVal);
		m_ObjUniformBuffer.SetVariable(1, projMatVal);
		m_ObjUniformBuffer.SetVariable(2, worldMatVal);

		// Light Buffer
		LightShaderResource lightShaderRes;
		lightShaderRes.m_Position = m_LightEntry->m_Transform->GetRawPtr()->GetPosition();
		lightShaderRes.m_Color = m_LightEntry->m_Light->GetRawPtr()->GetColor();
		lightShaderRes.m_Direction = m_LightEntry->m_Light->GetRawPtr()->GetDirection();
		lightShaderRes.m_Intensity = m_LightEntry->m_Light->GetRawPtr()->GetIntensity();
		//lightShaderRes.m_Type = (int)m_LightEntry->m_Light->GetRawPtr()->GetType();
		Core::ShaderVariable lightShaderVar(Core::VALUE_STRUCT, &lightShaderRes, sizeof(LightShaderResource));
		Core::ShaderVariable eyePos(Core::VALUE_VEC3, glm::value_ptr(m_MainCameraTransform->GetRawPtr()->GetPosition()), sizeof(vec3));

		m_LightUniformBuffer.SetVariable(0, lightShaderVar);
		m_LightUniformBuffer.SetVariable(1, eyePos);
		Core::Shader* shader = mesh->GetShader().get();
		shader->Activate();

		// Material Buffer
		Core::Material* matRes = mesh->GetMaterialProperties().get();
		MaterialShaderResource matShaderRes(*matRes);
		Core::ShaderVariable matShader(Core::VALUE_STRUCT, &matShaderRes, sizeof(MaterialShaderResource));
		m_MatUniformBuffer.SetVariable(0, matShader);

		if (!it->second.m_Init)
		{
			shader->InitializeUniformBuffer("ObjectBuffer", m_ObjUniformBuffer, VERTEX_PROGRAM);
			shader->InitializeUniformBuffer("LightBuffer", m_LightUniformBuffer, FRAGMENT_PROGRAM);
			shader->InitializeUniformBuffer("MaterialBuffer", m_MatUniformBuffer, FRAGMENT_PROGRAM);
			it->second.m_Init = true;
		}
		
		// Bind Samplers and Textures
		Core::Texture2D* diffuseTex = mesh->GetTexture(Core::DIFFUSE_MAP_IDX).get();
		LuxAssert(diffuseTex);

		Core::TextureSampler* texSampler = diffuseTex->GetSampler().get();
		LuxAssert(texSampler);

		texSampler->Activate(Core::DIFFUSE_MAP_IDX, FRAGMENT_PROGRAM);
		diffuseTex->Bind(Core::DIFFUSE_MAP_IDX, "DiffuseTexture", shader, FRAGMENT_PROGRAM);

		shader->Update();

		mesh->PreRender();
		m_RenderWindow->Render(mesh);
		mesh->PostRender();

		diffuseTex->Unbind();
		texSampler->Deactivate();
		shader->Deactivate();
	}
}

void Lux::Graphics::RenderingSystem::MainCameraSet(void* a_Ptr)
{
	Core::ObjectHandle<Core::Entity>* entity = (Core::ObjectHandle<Core::Entity>*)a_Ptr;

	m_MainCamera = &m_SceneManager->GetComponent<Camera>(*entity);
	m_MainCameraTransform = &m_SceneManager->GetComponent<Core::Transform>(*entity);
}
