#include "TestApp.h"
//#include "vld.h"
TestApp::TestApp()
{

}

TestApp::~TestApp()
{

}

bool TestApp::LoadContent()
{
	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../Assets"))
		return false;

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../Assets/models"))
		return false;

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../Assets/textures"))
		return false;

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../Assets/shaders"))
		return false;

	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("../Assets/models/California"))
		return false;

	// Load resources
	Lux::Core::ObserverPtr<Lux::Core::Shader> defaultShader = m_ResourceHandler->CreateShaderFromFile("Default.shader", "Default");
	Lux::Core::ObserverPtr<Lux::Core::Mesh> carMesh = m_ResourceHandler->CreateMeshFromFile("california.3ds", "Car", LuxProcess_CalcTangentSpace | LuxProcess_Triangulate | LuxProcess_JoinIdenticalVertices | LuxProcess_GenSmoothNormals);
	Lux::Core::ObserverPtr<Lux::Core::Mesh> cubeMesh = m_ResourceHandler->CreateMeshFromFile("cube.obj", "Cube", LuxProcess_CalcTangentSpace | LuxProcess_Triangulate | LuxProcess_JoinIdenticalVertices | LuxProcess_GenSmoothNormals);

	//Lux::Core::ObserverPtr<Lux::Core::Texture2D> carDiffuseTex = m_ResourceHandler->CreateTexture2DFromFile("UV_mapper.png", "CarTexture");
	Lux::Core::ObserverPtr<Lux::Core::Texture2D> groundDiffuseTex = m_ResourceHandler->CreateTexture2DFromFile("UV_mapper.png", "GroundTexture");
	Lux::Core::ObserverPtr<Lux::Core::TextureSampler> defaultSampler = m_ResourceHandler->CreateTextureSampler("DefaultSampler", Lux::Core::TextureSamplerOptions());
	Lux::Core::ObserverPtr<Lux::Core::Material>& defaultMaterial = m_ResourceHandler->CreateMaterial("TestMat");
	//carDiffuseTex->SetSampler(defaultSampler);
	groundDiffuseTex->SetSampler(defaultSampler);

	//carMesh.get()->SetDiffuseTexture(carDiffuseTex);
	carMesh.get()->SetMaterialProperties(defaultMaterial);
	carMesh.get()->SetShader(defaultShader);

	cubeMesh.get()->SetTexture(Lux::Core::DIFFUSE_MAP_IDX, groundDiffuseTex);
	cubeMesh.get()->SetMaterialProperties(defaultMaterial);
	cubeMesh.get()->SetShader(defaultShader);

	// Camera setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& cameraObj = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& cameraTransform = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(cameraObj);
	Lux::Core::ObjectHandle<Lux::Graphics::Camera>& mainCam = m_SceneManager->AttachNewComponent<Lux::Graphics::Camera>(cameraObj);
	mainCam.GetRawPtr()->SetProjectionPersp(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	mainCam.GetRawPtr()->SetMainCamera(true);
	cameraTransform.GetRawPtr()->SetPosition(Lux::vec3(0, 0, 10));

	// Add behaviour to the camera
	m_SceneManager->AttachNewComponent<Lux::Core::FreeLookCamera>(cameraObj);

	// Light setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& lightEntity = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& lightTransform = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(lightEntity);
	Lux::Core::ObjectHandle<Lux::Graphics::Light>& dirLight = m_SceneManager->AttachNewComponent<Lux::Graphics::Light>(lightEntity);
	lightTransform.GetRawPtr()->SetPosition(Lux::vec3(0, 100, 0));
	dirLight.GetRawPtr()->SetDirection(Lux::normalize(Lux::vec3(0.0, -1.0, 0)));

	// Test object setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& carEntity = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& carTransf = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(carEntity);
	//transf.GetRawPtr()->Rotate(180.0f, Lux::vec3(0, 1, 0));
	carTransf.GetRawPtr()->Rotate(90.0f, Lux::vec3(1, 0, 0));
	carTransf.GetRawPtr()->Rotate(180.0f, Lux::vec3(0, 0, 1));
	//carTransf.GetRawPtr()->SetScale(Lux::vec3(0.01f));
	carTransf.GetRawPtr()->SetPosition(Lux::vec3(0, 0, 0));

	// Make sure we have each sub mesh as a separate entity
	unsigned int numSubMeshes = carMesh.get()->GetNumSubMeshes();
	for (unsigned int i = 0; i < numSubMeshes; i++)
	{
		Lux::Core::ObjectHandle<Lux::Core::Entity>& tempEntity = m_SceneManager->CreateEntity();
		Lux::Core::ObjectHandle<Lux::Core::Transform>& tempTransf = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(tempEntity);
		tempTransf.GetRawPtr()->SetParentTransform(carTransf);
		Lux::Core::ObjectHandle<Lux::Graphics::MeshRenderer>& tempMeshRenderer = m_SceneManager->AttachNewComponent<Lux::Graphics::MeshRenderer>(tempEntity);
		tempMeshRenderer.GetRawPtr()->SetMesh(carMesh.get()->GetSubMesh(i));
	}

	// Add Physics
	Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> defaultPhysMat = m_ResourceHandler->CreatePhysicsMaterial("DefaultMat", 0.1f, 0.5f, 0.5f);
	Lux::Core::ObjectHandle<Lux::Physics::DynamicRigidBody>& carRigidBody = m_SceneManager->AttachNewComponent<Lux::Physics::DynamicRigidBody>(carEntity);
	carRigidBody.GetRawPtr()->SetPhysicsMaterial(defaultPhysMat);
	Lux::Core::ObjectHandle<Lux::Physics::BoxCollider>& carCollider = m_SceneManager->AttachNewComponent<Lux::Physics::BoxCollider>(carEntity);
	carCollider.GetRawPtr()->SetHalfExtents(carMesh.get()->GetAABB().GetHalfExtents() * carTransf.GetRawPtr()->GetScale());

	Lux::Core::ObjectHandle<Lux::Core::Entity>& groundEntity = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& groundTransf = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(groundEntity);
	groundTransf.GetRawPtr()->SetScale(Lux::vec3(100.0f, 1.0f, 100.0f));
	groundTransf.GetRawPtr()->SetPosition(Lux::vec3(0, -5, 0));

	// Make sure we have each sub mesh as a separate entity
	numSubMeshes = cubeMesh.get()->GetNumSubMeshes();
	for (unsigned int i = 0; i < numSubMeshes; i++)
	{
		Lux::Core::ObjectHandle<Lux::Core::Entity>& tempEntity = m_SceneManager->CreateEntity();
		Lux::Core::ObjectHandle<Lux::Core::Transform>& tempTransf = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(tempEntity);
		tempTransf.GetRawPtr()->SetParentTransform(groundTransf);
		Lux::Core::ObjectHandle<Lux::Graphics::MeshRenderer>& tempMeshRenderer = m_SceneManager->AttachNewComponent<Lux::Graphics::MeshRenderer>(tempEntity);
		tempMeshRenderer.GetRawPtr()->SetMesh(cubeMesh.get()->GetSubMesh(i));
	}

	Lux::Core::ObjectHandle<Lux::Physics::StaticRigidBody>& groundRigidBody = m_SceneManager->AttachNewComponent<Lux::Physics::StaticRigidBody>(groundEntity);
	groundRigidBody.GetRawPtr()->SetPhysicsMaterial(defaultPhysMat);
	Lux::Core::ObjectHandle<Lux::Physics::BoxCollider>& groundCollider = m_SceneManager->AttachNewComponent<Lux::Physics::BoxCollider>(groundEntity);
	groundCollider.GetRawPtr()->SetHalfExtents(cubeMesh.get()->GetAABB().GetHalfExtents() * groundTransf.GetRawPtr()->GetScale());

	return true;
}

bool TestApp::UnloadContent()
{
	return true;
}

bool TestApp::Update(const float a_DeltaTime)
{
	return true;
}

bool TestApp::LoadComponentTypes()
{
	return Lux::Application::LoadComponentTypes();
}

bool TestApp::LoadSystemTypes()
{
	return Lux::Application::LoadSystemTypes();
}
