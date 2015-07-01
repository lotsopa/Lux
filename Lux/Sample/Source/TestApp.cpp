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
	if (!Lux::Core::FileHandler::GetInstance().AddResourcePath("..\\Assets"))
	{
		return false;
	}

	// Load resources
	Lux::Core::ObserverPtr<Lux::Core::Shader> shader = m_ResourceHandler->CreateShaderFromFile("Test.shader", "Test");
	Lux::Core::ObserverPtr<Lux::Core::Mesh> mesh = m_ResourceHandler->CreateMeshFromFile("jeep.obj", "Monkey", LuxProcess_CalcTangentSpace | LuxProcess_Triangulate | LuxProcess_JoinIdenticalVertices | LuxProcess_GenSmoothNormals);

	Lux::Core::ObserverPtr<Lux::Core::Texture2D> diffuseTex = m_ResourceHandler->CreateTexture2DFromFile("jeep_army.png", "TestTexture");
	Lux::Core::ObserverPtr<Lux::Core::TextureSampler> defaultSampler = m_ResourceHandler->CreateTextureSampler("DefaultSampler", Lux::Core::TextureSamplerOptions());
	diffuseTex->SetSampler(defaultSampler);

	// Camera setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& cameraObj = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& cameraTransform = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(cameraObj);
	Lux::Core::ObjectHandle<Lux::Graphics::Camera>& mainCam = m_SceneManager->AttachNewComponent<Lux::Graphics::Camera>(cameraObj);
	mainCam.GetRawPtr()->SetProjectionPersp(60.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	mainCam.GetRawPtr()->SetMainCamera(true);
	cameraTransform.GetRawPtr()->SetPosition(Lux::vec3(0, 2, 5));

	// Add behaviour to the camera
	m_SceneManager->AttachNewComponent<Lux::Core::FreeLookCamera>(cameraObj);

	// Light setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& lightEntity = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& lightTransform = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(lightEntity);
	Lux::Core::ObjectHandle<Lux::Graphics::Light>& dirLight = m_SceneManager->AttachNewComponent<Lux::Graphics::Light>(lightEntity);
	lightTransform.GetRawPtr()->SetPosition(Lux::vec3(0, 0, 5));

	// Test object setup
	Lux::Core::ObjectHandle<Lux::Core::Entity>& ent = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& transf = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(ent);
	//transf.GetRawPtr()->Rotate(180.0f, Lux::vec3(0, 1, 0));
	//transf.GetRawPtr()->Rotate(90.0f, Lux::vec3(1, 0, 0));
	transf.GetRawPtr()->SetScale(Lux::vec3(0.01f));
	transf.GetRawPtr()->SetPosition(Lux::vec3(-4, 0, 0));
	Lux::Core::ObjectHandle<Lux::Graphics::MeshRenderer>& meshRenderer = m_SceneManager->AttachNewComponent<Lux::Graphics::MeshRenderer>(ent);
	meshRenderer.GetRawPtr()->SetMesh(mesh);
	Lux::Core::ObjectHandle<Lux::Graphics::Material>& materialComp = m_SceneManager->AttachNewComponent<Lux::Graphics::Material>(ent);
	materialComp.GetRawPtr()->SetShader(shader);
	materialComp.GetRawPtr()->SetDiffuseTexture(diffuseTex);

	// Add Physics
	Lux::Core::ObserverPtr<Lux::Core::PhysicsMaterial> physMat = m_ResourceHandler->CreatePhysicsMaterial("DefaultMat", 0.1f, 0.5f, 0.5f);
	Lux::Core::ObjectHandle<Lux::Physics::DynamicRigidBody>& rigidBody = m_SceneManager->AttachNewComponent<Lux::Physics::DynamicRigidBody>(ent);
	rigidBody.GetRawPtr()->SetPhysicsMaterial(physMat);
	Lux::Core::ObjectHandle<Lux::Physics::BoxCollider>& collider = m_SceneManager->AttachNewComponent<Lux::Physics::BoxCollider>(ent);

	Lux::Core::ObjectHandle<Lux::Core::Entity>& ent2 = m_SceneManager->CreateEntity();
	Lux::Core::ObjectHandle<Lux::Core::Transform>& transf2 = m_SceneManager->AttachNewComponent<Lux::Core::Transform>(ent2);
	transf2.GetRawPtr()->SetScale(Lux::vec3(0.01f));
	transf2.GetRawPtr()->SetPosition(Lux::vec3(0, -5, 0));
	Lux::Core::ObjectHandle<Lux::Graphics::MeshRenderer>& meshRenderer2 = m_SceneManager->AttachNewComponent<Lux::Graphics::MeshRenderer>(ent2);
	meshRenderer2.GetRawPtr()->SetMesh(mesh);
	Lux::Core::ObjectHandle<Lux::Graphics::Material>& materialComp2 = m_SceneManager->AttachNewComponent<Lux::Graphics::Material>(ent2);
	materialComp2.GetRawPtr()->SetShader(shader);
	materialComp2.GetRawPtr()->SetDiffuseTexture(diffuseTex);

	Lux::Core::ObjectHandle<Lux::Physics::StaticRigidBody>& rigidBody2 = m_SceneManager->AttachNewComponent<Lux::Physics::StaticRigidBody>(ent2);
	rigidBody2.GetRawPtr()->SetPhysicsMaterial(physMat);
	Lux::Core::ObjectHandle<Lux::Physics::BoxCollider>& collider2 = m_SceneManager->AttachNewComponent<Lux::Physics::BoxCollider>(ent2);

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
