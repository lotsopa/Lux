#ifndef LUX_PHYSICS_SYSTEM_H
#define LUX_PHYSICS_SYSTEM_H
#include "LuxPhysicsMaterialComponent.h"

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
				EntityEntry() : m_Material(nullptr)
				{

				}

				~EntityEntry()
				{

				}

				bool IsNull()
				{
					if (!m_Material)
						return true;

					if (m_Material)
					{
						if (m_Material->IsValid())
							return false;
					}

					return true;
				}

				Core::ObjectHandle<PhysicsMaterialComponent>* m_Material;
			};
			typedef std::map<Core::ObjectHandle<Core::Entity>*, EntityEntry> EntityMap;
			typedef std::map<Core::Key, std::function<void(void*, Core::ObjectHandle<Core::Entity>&)>> AddComponentProcessMap;
			typedef std::map<Core::Key, std::function<void(Core::ObjectHandle<Core::Entity>&)>> RemoveComponentProcessMap;
			Core::Key m_MaterialKey;
			AddComponentProcessMap m_AddComponentFuncMap;
			EntityMap m_EntityMap;
			RemoveComponentProcessMap m_RemoveComponentProcessMap;

			bool EntityEntryExists(Core::ObjectHandle<Core::Entity>& a_Entity);

			// Add Components
			template<class ComponentType>
			void AddComponentInternal(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			//Specializations
			template<> void AddComponentInternal<PhysicsMaterialComponent>(void* a_CompPtr, Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Core::ObjectHandle<PhysicsMaterialComponent>* compPtr = (Core::ObjectHandle<PhysicsMaterialComponent>*)(a_CompPtr);
				compPtr->GetRawPtr()->m_Properties = m_Physics->createMaterial(0.0f, 0.0f, 0.0f);

				if (!compPtr->GetRawPtr()->m_Properties)
					Utility::ThrowError("Failed to create PxMaterial.");

				m_EntityMap[&a_Owner].m_Material = compPtr;
			}

			// Remove Components
			template<class ComponentType>
			void RemoveComponentInternal(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				Utility::ThrowError("Could not add Component to Rendering System. Not supported.");
			}

			// Specializations
			template<> void RemoveComponentInternal<PhysicsMaterialComponent>(Core::ObjectHandle<Core::Entity>& a_Owner)
			{
				m_EntityMap[&a_Owner].m_Material = nullptr;
			}

		};
	}
}

#endif