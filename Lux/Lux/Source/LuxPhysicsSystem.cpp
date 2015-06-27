#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxSubMesh.h"
#include "LuxMesh.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxTimer.h"
#include "LuxPhysicsSystem.h"

Lux::Physics::PhysicsSystem::PhysicsSystem() : System(), m_RecordAllocations(true), m_NumPhysicsThreads(1), m_StepTimeSec(0.1f)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	if (!m_Foundation)
		Utility::ThrowError("PxCreateFoundation failed!");

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), m_RecordAllocations);

	if (!m_Physics)
		Utility::ThrowError("PxCreatePhysics failed!");

	// Set up physics scene

	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(m_NumPhysicsThreads);

		if (!m_CpuDispatcher)
			Utility::ThrowError("PxDefaultCpuDispatcherCreate failed!");

		sceneDesc.cpuDispatcher = m_CpuDispatcher;
	}
	if (!sceneDesc.filterShader)
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

	m_Scene = m_Physics->createScene(sceneDesc);

	if (!m_Scene)
		Utility::ThrowError("createScene failed!");
}

Lux::Physics::PhysicsSystem::~PhysicsSystem()
{
	m_Scene->release();
	m_Physics->release();
	m_Foundation->release();
}

void Lux::Physics::PhysicsSystem::ProcessUpdate(const float a_DeltaTime)
{
	if (m_StepTimer.ElapsedSeconds(m_StepTimeSec))
	{
		m_Scene->simulate(m_StepTimeSec);

		//Make sure we get all results
		m_Scene->fetchResults(true);

		m_StepTimer.Reset();
	}
}

bool Lux::Physics::PhysicsSystem::Init(Core::SceneManager* a_SceneManager)
{
	Core::System::Init(a_SceneManager);
	return true;
}

void Lux::Physics::PhysicsSystem::AddComponent(void* a_Component, const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity)
{

}

void Lux::Physics::PhysicsSystem::RemoveComponent(const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity)
{

}

