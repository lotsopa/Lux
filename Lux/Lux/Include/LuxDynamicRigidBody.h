#ifndef LUX_DYNAMIC_RIGID_BODY_H
#define LUX_DYNAMIC_RIGID_BODY_H
#include "LuxComponent.h"
#include "LuxObserverPtr.h"

namespace Lux
{
	namespace Core
	{
		class PhysicsMaterial;
	}

	namespace Physics
	{
		class DynamicRigidBody : public Core::Component
		{
		public:
			DynamicRigidBody();
			DynamicRigidBody(const DynamicRigidBody& a_Other);
			virtual ~DynamicRigidBody();

			inline void SetPhysicsMaterial(Core::ObserverPtr<Core::PhysicsMaterial>& a_Material)
			{
				m_Material.reset(a_Material.get()); m_Dirty = true;
			}

			inline Core::ObserverPtr<Core::PhysicsMaterial>& GetPhysicsMaterial()
			{
				return m_Material;
			}

		private:
			PxRigidDynamic* m_Properties;
			Core::ObserverPtr<Core::PhysicsMaterial> m_Material;
			friend class PhysicsSystem;
			bool m_Dirty;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif