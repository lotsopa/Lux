#ifndef LUX_PHYSICS_SYSTEM_H
#define LUX_PHYSICS_SYSTEM_H

namespace Lux
{
	namespace Core
	{
		class SceneManager;
		class Transform;
		class Key;
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
		};
	}
}

#endif