#ifndef LUX_PHYSICS_SYSTEM_H
#define LUX_PHYSICS_SYSTEM_H
#include "LuxPhysicsMaterial.h"
#include "LuxDynamicRigidBody.h"
#include "LuxStaticRigidBody.h"
#include "LuxBoxCollider.h"
#include "LuxSphereCollider.h"
#include "LuxCapsuleCollider.h"
#include "LuxTransform.h"
#include "LuxMeshRenderer.h"

namespace Lux
{
	namespace Core
	{
		class SceneManager;
		class Transform;
		class Key;
		class PhysicsMaterial;
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

			void MeshSet(void* a_Ptr);
			void PhysMatSet(void* a_Ptr);

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
				EntityEntry() : m_RigidBody(nullptr), m_Transform(nullptr),
					m_Collider(nullptr)
				{

				}

				~EntityEntry()
				{

				}

				bool IsNull()
				{
					if (!m_RigidBody && !m_Transform)
						return true;

					if (m_RigidBody)
					{
						if (m_RigidBody->IsValid())
							return false;
					}

					if (m_Transform)
					{
						if (m_Transform->IsValid())
							return false;
					}

					if (m_Collider)
					{
						if (m_Collider->IsValid())
							return false;
					}

					return true;
				}

				Core::ObjectHandle<RigidBody>* m_RigidBody;
				Core::ObjectHandle<Core::Transform>* m_Transform;
				Core::ObjectHandle<Collider>* m_Collider;
			};
			typedef std::map<Core::ObjectHandle<Core::Entity>*, EntityEntry> EntityMap;
			typedef std::map<Core::Key, std::function<void(void*, Core::ObjectHandle<Core::Entity>&)>> AddComponentProcessMap;
			typedef std::map<Core::Key, std::function<void(Core::ObjectHandle<Core::Entity>&)>> RemoveComponentProcessMap;
			Core::Key m_MaterialKey;
			Core::Key m_DynamicRigidBodyKey;
			Core::Key m_StaticRigidBodyKey;
			Core::Key m_TransformKey;
			Core::Key m_BoxColliderKey;
			Core::Key m_SphereColliderKey;
			Core::Key m_CapsuleColliderKey;
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
				DynamicRigidBody* rigidBody = compPtr->GetRawPtr();
				rigidBody->m_Properties = m_Physics->createRigidDynamic(physTransform);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create DynamicRigidBody.");

				m_EntityMap[&a_Owner].m_RigidBody = (Core::ObjectHandle<RigidBody>*)compPtr;
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
				StaticRigidBody* rigidBody = compPtr->GetRawPtr();
				rigidBody->m_Properties = m_Physics->createRigidStatic(physTransform);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create StaticRigidBody.");

				m_EntityMap[&a_Owner].m_RigidBody = (Core::ObjectHandle<RigidBody>*)compPtr;
				//m_Scene->addActor(*compPtr->GetRawPtr()->m_Properties);
			}

			template<> void AddComponentInternal<Core::Transform>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = (Core::ObjectHandle<Lux::Core::Transform>*)(a_CompPtr);
			}

			template<> void AddComponentInternal<BoxCollider>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_Collider)
				{
					Utility::ThrowError("Failed to create Box Collider. The entity already has a Collider attached.");
				}

				Core::ObjectHandle<Core::Transform>& transformHandle = m_SceneManager->GetComponent<Core::Transform>(a_Owner);

				if (!transformHandle.IsValid())
				{
					Utility::ThrowError("Failed to create Box Collider. The entity must have a transform.");
				}

				Core::ObjectHandle<Collider>* colliderHandle = (Core::ObjectHandle<Collider>*)(a_CompPtr);
				BoxCollider* colliderPtr = (BoxCollider*)colliderHandle->GetRawPtr();

				// Set the bounding box, if the entity has one
				if (m_SceneManager->HasComponent<Graphics::MeshRenderer>(a_Owner))
				{
					Core::ObjectHandle<Graphics::MeshRenderer>& meshRenderer = m_SceneManager->GetComponent<Graphics::MeshRenderer>(a_Owner);

					if (meshRenderer.IsValid())
					{
						Core::AABB& aabb = meshRenderer.GetRawPtr()->GetMesh().get()->GetAABB();
						colliderPtr->m_HalfExtents = aabb.GetHalfExtents() * transformHandle.GetRawPtr()->GetScale();
					}
				}
				
				
				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					RigidBody* rigidBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (!colliderPtr->m_Shape)
					{
						if (rigidBody->m_Material)
						{
							// Create material
							Core::PhysicsMaterial* mat = rigidBody->m_Material.get();
							if (!mat->m_Properties)
							{
								mat->m_Properties = m_Physics->createMaterial(mat->m_StaticFriction, mat->m_DynamicFriction, mat->m_Restitution);

								if (!mat->m_Properties)
									Utility::ThrowError("Failed to create Physics Material.");
							}

							colliderPtr->m_Shape = m_Physics->createShape(PxBoxGeometry(Utility::ConvertVec3ToPhysX(colliderPtr->m_HalfExtents)), *mat->m_Properties, true);

							if (!colliderPtr->m_Shape)
								Utility::ThrowError("Unable to create Box Collider. Unable to create shape.");
						}
						else
						{
							Utility::ThrowError("Unable to create Box Collider. The RigidBody must have a material.");
						}
					}

					RigidBody* rBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (rBody->m_Properties)
					{
						rBody->m_Properties->attachShape(*colliderPtr->m_Shape);

						if (rBody->m_Type == RigidBody::RIGID_BODY_STATIC)
							m_Scene->addActor(*rBody->m_Properties);
					}

				}
				m_EntityMap[&a_Owner].m_Collider = colliderHandle;
			}

			template<> void AddComponentInternal<SphereCollider>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_Collider)
				{
					Utility::ThrowError("Failed to create Sphere Collider. The entity already has a Collider attached.");
				}

				Core::ObjectHandle<Core::Transform>& transformHandle = m_SceneManager->GetComponent<Core::Transform>(a_Owner);

				if (!transformHandle.IsValid())
				{
					Utility::ThrowError("Failed to create Sphere Collider. The entity must have a transform.");
				}

				Core::ObjectHandle<Collider>* colliderHandle = (Core::ObjectHandle<Collider>*)(a_CompPtr);
				SphereCollider* colliderPtr = (SphereCollider*)colliderHandle->GetRawPtr();


				// Set the bounding box, if the entity has one
				if (m_SceneManager->HasComponent<Graphics::MeshRenderer>(a_Owner))
				{
					Core::ObjectHandle<Graphics::MeshRenderer>& meshRenderer = m_SceneManager->GetComponent<Graphics::MeshRenderer>(a_Owner);

					if (meshRenderer.IsValid())
					{
						Core::AABB& aabb = meshRenderer.GetRawPtr()->GetMesh().get()->GetAABB();

						// Pick longest axis
						vec3 halfExtents = aabb.GetHalfExtents() * transformHandle.GetRawPtr()->GetScale();
						float longestAxis = std::max(halfExtents.x, std::max(halfExtents.y, halfExtents.z));
						colliderPtr->m_Radius = longestAxis;
					}
				}

				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					RigidBody* rigidBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (!colliderPtr->m_Shape)
					{
						if (rigidBody->m_Material)
						{
							// Create material
							Core::PhysicsMaterial* mat = rigidBody->m_Material.get();
							if (!mat->m_Properties)
							{
								mat->m_Properties = m_Physics->createMaterial(mat->m_StaticFriction, mat->m_DynamicFriction, mat->m_Restitution);

								if (!mat->m_Properties)
									Utility::ThrowError("Failed to create Physics Material.");
							}

							colliderPtr->m_Shape = m_Physics->createShape(PxSphereGeometry(colliderPtr->m_Radius), *mat->m_Properties, true);

							if (!colliderPtr->m_Shape)
								Utility::ThrowError("Unable to create Box Collider. Unable to create shape.");
						}
						else
						{
							Utility::ThrowError("Unable to create Box Collider. The RigidBody must have a material.");
						}
					}

					RigidBody* rBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (rBody->m_Properties)
					{
						rBody->m_Properties->attachShape(*colliderPtr->m_Shape);

						if (rBody->m_Type == RigidBody::RIGID_BODY_STATIC)
							m_Scene->addActor(*rBody->m_Properties);
					}

				}
				m_EntityMap[&a_Owner].m_Collider = colliderHandle;
			}

			template<> void AddComponentInternal<CapsuleCollider>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_Collider)
				{
					Utility::ThrowError("Failed to create Sphere Collider. The entity already has a Collider attached.");
				}

				Core::ObjectHandle<Core::Transform>& transformHandle = m_SceneManager->GetComponent<Core::Transform>(a_Owner);

				if (!transformHandle.IsValid())
				{
					Utility::ThrowError("Failed to create Sphere Collider. The entity must have a transform.");
				}

				Core::ObjectHandle<Collider>* colliderHandle = (Core::ObjectHandle<Collider>*)(a_CompPtr);
				CapsuleCollider* colliderPtr = (CapsuleCollider*)colliderHandle->GetRawPtr();

				PxTransform relativePose;
				if (m_SceneManager->HasComponent<Graphics::MeshRenderer>(a_Owner))
				{
					// Set the bounding box, if the entity has one
					Core::ObjectHandle<Graphics::MeshRenderer>& meshRenderer = m_SceneManager->GetComponent<Graphics::MeshRenderer>(a_Owner);
					if (meshRenderer.IsValid())
					{
						Core::AABB& aabb = meshRenderer.GetRawPtr()->GetMesh().get()->GetAABB();

						// Pick longest axis
						vec3 halfExtents = aabb.GetHalfExtents() * transformHandle.GetRawPtr()->GetScale();
						float longestAxis = std::max(halfExtents.x, std::max(halfExtents.y, halfExtents.z));
						colliderPtr->m_HalfHeight = longestAxis;

						// Make sure we encompass the whole mesh
						if (longestAxis == halfExtents.x)
						{
							colliderPtr->m_Orientation = CapsuleCollider::X_AXIS;
							relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(1, 0, 0)));
							colliderPtr->m_Radius = halfExtents.z;
						}
						else if (longestAxis == halfExtents.y)
						{
							colliderPtr->m_Orientation = CapsuleCollider::Y_AXIS;
							relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
							colliderPtr->m_Radius = halfExtents.x;
						}
						else
						{
							colliderPtr->m_Orientation = CapsuleCollider::Z_AXIS;
							relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(0, 1, 0)));
							colliderPtr->m_Radius = halfExtents.y;
						}
					}
				}

				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					RigidBody* rigidBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (!colliderPtr->m_Shape)
					{
						if (rigidBody->m_Material)
						{
							// Create material
							Core::PhysicsMaterial* mat = rigidBody->m_Material.get();
							if (!mat->m_Properties)
							{
								mat->m_Properties = m_Physics->createMaterial(mat->m_StaticFriction, mat->m_DynamicFriction, mat->m_Restitution);

								if (!mat->m_Properties)
									Utility::ThrowError("Failed to create Physics Material.");
							}

							colliderPtr->m_Shape = m_Physics->createShape(PxCapsuleGeometry(colliderPtr->m_Radius, colliderPtr->m_HalfHeight), *mat->m_Properties, true);
							colliderPtr->m_Shape->setLocalPose(relativePose);

							if (!colliderPtr->m_Shape)
								Utility::ThrowError("Unable to create Box Collider. Unable to create shape.");
						}
						else
						{
							Utility::ThrowError("Unable to create Box Collider. The RigidBody must have a material.");
						}
					}

					RigidBody* rBody = m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
					if (rBody->m_Properties)
					{
						rBody->m_Properties->attachShape(*colliderPtr->m_Shape);

						if (rBody->m_Type == RigidBody::RIGID_BODY_STATIC)
							m_Scene->addActor(*rBody->m_Properties);
					}

				}
				m_EntityMap[&a_Owner].m_Collider = colliderHandle;
			}

			// Remove Components
			template<class ComponentType>
			void RemoveComponentInternal(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			// Specializations
			template<> void RemoveComponentInternal<DynamicRigidBody>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				DynamicRigidBody* rBody = (DynamicRigidBody*)m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
				m_Scene->removeActor(*rBody->m_Properties);
				m_EntityMap[&a_Owner].m_RigidBody = nullptr;
			}

			template<> void RemoveComponentInternal<StaticRigidBody>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				StaticRigidBody* rBody = (StaticRigidBody*)m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr();
				m_Scene->removeActor(*rBody->m_Properties);
				m_EntityMap[&a_Owner].m_RigidBody = nullptr;
			}

			template<> void RemoveComponentInternal<Core::Transform>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Transform = nullptr;
			}

			template<> void RemoveComponentInternal<BoxCollider>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr()->m_Properties->detachShape(*m_EntityMap[&a_Owner].m_Collider->GetRawPtr()->m_Shape);
				}

				m_EntityMap[&a_Owner].m_Collider = nullptr;
			}

			template<> void RemoveComponentInternal<SphereCollider>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr()->m_Properties->detachShape(*m_EntityMap[&a_Owner].m_Collider->GetRawPtr()->m_Shape);
				}

				m_EntityMap[&a_Owner].m_Collider = nullptr;
			}

			template<> void RemoveComponentInternal<CapsuleCollider>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				if (m_EntityMap[&a_Owner].m_RigidBody != nullptr && m_EntityMap[&a_Owner].m_RigidBody->IsValid())
				{
					m_EntityMap[&a_Owner].m_RigidBody->GetRawPtr()->m_Properties->detachShape(*m_EntityMap[&a_Owner].m_Collider->GetRawPtr()->m_Shape);
				}

				m_EntityMap[&a_Owner].m_Collider = nullptr;
			}
		};
	}
}

#endif