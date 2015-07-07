#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxEntity.h"
#include "LuxTransform.h"
#include "LuxKey.h"
#include "LuxMesh.h"
#include "LuxModel.h"
#include "LuxObjectPool.h"
#include "LuxComponentFactory.h"
#include "LuxEntityFactory.h"
#include "LuxSceneManager.h"
#include "LuxSystem.h"
#include "LuxTimer.h"
#include "LuxPhysicsSystem.h"

#define CONVERT_ID_TO_CLASS_STRING(a) "class " ID_TO_STRING(a)
#define ADD_COMPONENT_MAP_INSERT(a, b) m_AddComponentFuncMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1, std::placeholders::_2)))
#define REMOVE_COMPONENT_MAP_INSERT(a, b) m_RemoveComponentProcessMap.insert(std::make_pair(a, std::bind(&b, this, std::placeholders::_1)))

Lux::Physics::PhysicsSystem::PhysicsSystem() : System(), m_RecordAllocations(true), m_NumPhysicsThreads(LUX_NUM_PHYSICS_THREADS), m_StepTimeSec(LUX_PHYSICS_TIMESTEP_SEC),
m_DynamicRigidBodyKey(CONVERT_ID_TO_CLASS_STRING(Lux::Physics::DynamicRigidBody)),
m_StaticRigidBodyKey(CONVERT_ID_TO_CLASS_STRING(Lux::Physics::StaticRigidBody)),
m_TransformKey(CONVERT_ID_TO_CLASS_STRING(Lux::Core::Transform)),
m_BoxColliderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Physics::BoxCollider)),
m_SphereColliderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Physics::SphereCollider)),
m_CapsuleColliderKey(CONVERT_ID_TO_CLASS_STRING(Lux::Physics::CapsuleCollider))
{
	ADD_COMPONENT_MAP_INSERT(m_StaticRigidBodyKey, PhysicsSystem::AddComponentInternal<StaticRigidBody>);
	ADD_COMPONENT_MAP_INSERT(m_DynamicRigidBodyKey, PhysicsSystem::AddComponentInternal<DynamicRigidBody>);
	ADD_COMPONENT_MAP_INSERT(m_TransformKey, PhysicsSystem::AddComponentInternal<Core::Transform>);
	ADD_COMPONENT_MAP_INSERT(m_BoxColliderKey, PhysicsSystem::AddComponentInternal<BoxCollider>);
	ADD_COMPONENT_MAP_INSERT(m_SphereColliderKey, PhysicsSystem::AddComponentInternal<SphereCollider>);
	ADD_COMPONENT_MAP_INSERT(m_CapsuleColliderKey, PhysicsSystem::AddComponentInternal<CapsuleCollider>);

	REMOVE_COMPONENT_MAP_INSERT(m_StaticRigidBodyKey, PhysicsSystem::RemoveComponentInternal<StaticRigidBody>);
	REMOVE_COMPONENT_MAP_INSERT(m_DynamicRigidBodyKey, PhysicsSystem::RemoveComponentInternal<DynamicRigidBody>);
	REMOVE_COMPONENT_MAP_INSERT(m_TransformKey, PhysicsSystem::RemoveComponentInternal<Core::Transform>);
	REMOVE_COMPONENT_MAP_INSERT(m_BoxColliderKey, PhysicsSystem::RemoveComponentInternal<BoxCollider>);
	REMOVE_COMPONENT_MAP_INSERT(m_SphereColliderKey, PhysicsSystem::RemoveComponentInternal<SphereCollider>);
	REMOVE_COMPONENT_MAP_INSERT(m_CapsuleColliderKey, PhysicsSystem::RemoveComponentInternal<CapsuleCollider>);

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	if (!m_Foundation)
		Utility::ThrowError("PxCreateFoundation failed!");

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, physx::PxTolerancesScale(), m_RecordAllocations);

	if (!m_Physics)
		Utility::ThrowError("PxCreatePhysics failed!");

	// Set up physics scene

	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = Utility::ConvertVec3ToPhysX(LUX_WORLD_GRAVITY);

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
	Utility::SafePtrDelete(m_CpuDispatcher);

	if (m_Scene)
		m_Scene->release();

	if (m_Physics)
		m_Physics->release();

	if (m_Foundation)
		m_Foundation->release();
}

void Lux::Physics::PhysicsSystem::ProcessUpdate(const float a_DeltaTime)
{
	// Process and update components if necessary
	ProcessComponents();

	if (m_StepTimer.ElapsedSeconds(m_StepTimeSec))
	{
		m_Scene->simulate(m_StepTimeSec);

		//Make sure we get all results
		if (m_Scene->fetchResults(true))
		{
			// Make sure all entities get their transforms updated
			UpdateObjectPositions();
		}

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
	m_AddComponentFuncMap[a_CompType](a_Component, a_Entity);
}

void Lux::Physics::PhysicsSystem::RemoveComponent(const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity)
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

bool Lux::Physics::PhysicsSystem::EntityEntryExists(Core::ObjectHandle<Core::Entity>& a_Entity)
{
	int count = m_EntityMap.count(&a_Entity);

	if (count > 0)
	{
		return true;
	}

	return false;
}

void Lux::Physics::PhysicsSystem::UpdateObjectPositions()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (it->second.IsNull())
			continue;

		if (!it->second.m_Transform)
			continue;

		if (!it->second.m_Transform->IsValid())
			continue;

		if (!it->second.m_RigidBody)
			continue;

		if (!it->second.m_RigidBody->IsValid())
			continue;

		if (it->second.m_RigidBody->GetRawPtr()->m_Type == RigidBody::RIGID_BODY_STATIC)
			continue;

		PxRigidActor* dynamicRigidBody = it->second.m_RigidBody->GetRawPtr()->m_Properties;
		PxTransform bodyTransform = dynamicRigidBody->getGlobalPose();

		Core::Transform* objTransform = it->second.m_Transform->GetRawPtr();

		objTransform->SetPosition(Utility::ConvertVec3PhysX(bodyTransform.p));
		objTransform->SetRotation(Utility::ConvertQuatPhysX(bodyTransform.q));
	}
}

void Lux::Physics::PhysicsSystem::ProcessComponents()
{
	EntityMap::iterator it;

	for (it = m_EntityMap.begin(); it != m_EntityMap.end(); ++it)
	{
		if (it->second.IsNull())
			continue;

		if (it->second.m_RigidBody)
		{
			if (it->second.m_RigidBody->IsValid())
			{
				RigidBody* rigidBody = it->second.m_RigidBody->GetRawPtr();
				// Check if we need to create a material
				if (rigidBody->m_Material)
				{
					Core::PhysicsMaterial* mat = rigidBody->m_Material.get();
					if (!mat->m_Properties)
					{
						mat->m_Properties = m_Physics->createMaterial(mat->m_StaticFriction, mat->m_DynamicFriction, mat->m_Restitution);

						if (!mat->m_Properties)
							Utility::ThrowError("Failed to create Physics Material.");
					}
				}
			}
		}
	}
}

