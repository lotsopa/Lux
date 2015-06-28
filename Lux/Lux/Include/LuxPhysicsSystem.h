#ifndef LUX_PHYSICS_SYSTEM_H
#define LUX_PHYSICS_SYSTEM_H
#include "LuxPhysicsMaterial.h"
#include "LuxDynamicRigidBody.h"
#include "LuxStaticRigidBody.h"
#include "LuxTransform.h"

namespace Lux
{
	namespace Core
	{
		class SceneManager;
		class Transform;
		class Key;
	}

	namespace Utility
	{
		class Timer;
	}

	namespace Physics
	{

		class PhysicsSystem : public Core::System
		{
		public:
			PhysicsSystem();
			~PhysicsSystem();

			virtual void ProcessUpdate(const float a_DeltaTime);
			virtual bool Init(Core::SceneManager* a_SceneManager);
			virtual void AddComponent(void* a_Component, const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity);
			virtual void RemoveComponent(const Core::Key& a_CompType, Core::ObjectHandle<Core::Entity>& a_Entity);

		private:
			physx::PxFoundation* m_Foundation;
			physx::PxDefaultErrorCallback m_ErrorCallback;
			physx::PxDefaultAllocator m_Allocator;
			physx::PxPhysics* m_Physics;
			bool m_RecordAllocations;
			unsigned int m_NumPhysicsThreads;
			physx::PxScene* m_Scene;
			physx::PxCpuDispatcher* m_CpuDispatcher;
			Utility::Timer m_StepTimer;
			float m_StepTimeSec;

			struct EntityEntry
			{
				EntityEntry() : m_Material(nullptr), m_DynamicRigidBody(nullptr), 
					m_StaticRigidBody(nullptr), m_Transform(nullptr)
				{

				}

				~EntityEntry()
				{

				}

				bool IsNull()
				{
					if (!m_Material && !m_DynamicRigidBody && !m_StaticRigidBody && !m_Transform)
						return true;

					if (m_Material)
					{
						if (m_Material->IsValid())
							return false;
					}

					if (m_DynamicRigidBody)
					{
						if (m_DynamicRigidBody->IsValid())
							return false;
					}

					if (m_StaticRigidBody)
					{
						if (m_StaticRigidBody->IsValid())
							return false;
					}

					if (m_Transform)
					{
						if (m_Transform->IsValid())
							return false;
					}

					return true;
				}

				Core::ObjectHandle<PhysicsMaterial>* m_Material;
				Core::ObjectHandle<DynamicRigidBody>* m_DynamicRigidBody;
				Core::ObjectHandle<StaticRigidBody>* m_StaticRigidBody;
				Core::ObjectHandle<Core::Transform>* m_Transform;
			};
			typedef std::map<Core::ObjectHandle<Core::Entity>*, EntityEntry> EntityMap;
			typedef std::map<Core::Key, std::function<void(void*, Core::ObjectHandle<Core::Entity>&)>> AddComponentProcessMap;
			typedef std::map<Core::Key, std::function<void(Core::ObjectHandle<Core::Entity>&)>> RemoveComponentProcessMap;
			Core::Key m_MaterialKey;
			Core::Key m_DynamicRigidBodyKey;
			Core::Key m_StaticRigidBodyKey;
			Core::Key m_TransformKey;
			AddComponentProcessMap m_AddComponentFuncMap;
			EntityMap m_EntityMap;
			RemoveComponentProcessMap m_RemoveComponentProcessMap;

			bool EntityEntryExists(Core::ObjectHandle<Core::Entity>& a_Entity);
			void UpdateObjectPositions();

			// Add Components
			template<class ComponentType>
			void AddComponentInternal(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			//Specializations
			template<> void AddComponentInternal<PhysicsMaterial>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Core::ObjectHandle<PhysicsMaterial>* compPtr = (Core::ObjectHandle<PhysicsMaterial>*)(a_CompPtr);
				compPtr->GetRawPtr()->m_Properties = m_Physics->createMaterial(0.0f, 0.0f, 0.0f);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create PxMaterial.");

				m_EntityMap[&a_Owner].m_Material = compPtr;
			}

			template<> void AddComponentInternal<DynamicRigidBody>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Core::ObjectHandle<Core::Transform>& transformHandle = m_SceneManager->GetComponent<Core::Transform>(a_Owner);
				
				if (!transformHandle.IsValid())
				{
					Utility::ThrowError("Failed to create Dynamic Rigid Body. The entity must have a transform.");
				}

				// Create a physics transform
				PxTransform physTransform;
				Core::Transform* transform = transformHandle.GetRawPtr();
				physTransform.p = Utility::ConvertVec3ToPhysX(transform->GetPosition());
				physTransform.q = Utility::ConvertQuatToPhysX(transform->GetRotation());

				Core::ObjectHandle<DynamicRigidBody>* compPtr = (Core::ObjectHandle<DynamicRigidBody>*)(a_CompPtr);
				compPtr->GetRawPtr()->m_Properties = m_Physics->createRigidDynamic(physTransform);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create PxMaterial.");

				m_EntityMap[&a_Owner].m_DynamicRigidBody = compPtr;
				m_Scene->addActor(*compPtr->GetRawPtr()->m_Properties);
			}

			template<> void AddComponentInternal<StaticRigidBody>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Core::ObjectHandle<Core::Transform>& transformHandle = m_SceneManager->GetComponent<Core::Transform>(a_Owner);

				if (!transformHandle.IsValid())
				{
					Utility::ThrowError("Failed to create Dynamic Rigid Body. The entity must have a transform.");
				}

				// Create a physics transform
				PxTransform physTransform;
				Core::Transform* transform = transformHandle.GetRawPtr();
				physTransform.p = Utility::ConvertVec3ToPhysX(transform->GetPosition());
				physTransform.q = Utility::ConvertQuatToPhysX(transform->GetRotation());

				Core::ObjectHandle<StaticRigidBody>* compPtr = (Core::ObjectHandle<StaticRigidBody>*)(a_CompPtr);
				compPtr->GetRawPtr()->m_Properties = m_Physics->createRigidStatic(physTransform);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create PxMaterial.");

				m_EntityMap[&a_Owner].m_StaticRigidBody = compPtr;
				m_Scene->addActor(*compPtr->GetRawPtr()->m_Properties);
			}

			template<> void AddComponentInternal<Core::Transform>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = (Core::ObjectHandle<Lux::Core::Transform>*)(a_CompPtr);
			}

			// Remove Components
			template<class ComponentType>
			void RemoveComponentInternal(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			// Specializations
			template<> void RemoveComponentInternal<PhysicsMaterial>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Material = nullptr;
			}

			// Specializations
			template<> void RemoveComponentInternal<DynamicRigidBody>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_Scene->removeActor(*m_EntityMap[&a_Owner].m_DynamicRigidBody->GetRawPtr()->m_Properties);
				m_EntityMap[&a_Owner].m_DynamicRigidBody = nullptr;
			}

			// Specializations
			template<> void RemoveComponentInternal<StaticRigidBody>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_Scene->removeActor(*m_EntityMap[&a_Owner].m_StaticRigidBody->GetRawPtr()->m_Properties);
				m_EntityMap[&a_Owner].m_StaticRigidBody = nullptr;
			}

			template<> void RemoveComponentInternal<Core::Transform>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = nullptr;
			}

		};
	}
}

#endif