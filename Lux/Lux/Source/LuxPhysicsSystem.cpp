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
#include "LuxPhysicsSystem.h"

Lux::Physics::PhysicsSystem::PhysicsSystem() : System(), m_RecordAllocations(true)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	if (!m_Foundation)
		Utility::ThrowError("PxCreateFoundation failed!");

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), m_RecordAllocations);

	if (!m_Physics)
		Utility::ThrowError("PxCreatePhysics failed!");
}

Lux::Physics::PhysicsSystem::~PhysicsSystem()
{
	m_Physics->release();
	m_Foundation->release();
}

void Lux::Physics::PhysicsSystem::ProcessUpdate(const float a_DeltaTime)
{

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

