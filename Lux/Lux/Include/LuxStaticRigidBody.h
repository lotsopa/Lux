#ifndef LUX_STATIC_RIGID_BODY_H
#define LUX_STATIC_RIGID_BODY_H

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
		class StaticRigidBody : public Core::Component
		{
		public:
			StaticRigidBody();
			StaticRigidBody(const StaticRigidBody& a_Other);
			virtual ~StaticRigidBody();

			inline void SetPhysicsMaterial(Core::ObserverPtr<Core::PhysicsMaterial>& a_Material)
			{
				m_Material.reset(a_Material.get()); m_Dirty = true;
			}

			inline Core::ObserverPtr<Core::PhysicsMaterial>& GetPhysicsMaterial()
			{
				return m_Material;
			}

		private:
			PxRigidStatic* m_Properties;
			Core::ObserverPtr<Core::PhysicsMaterial> m_Material;
			bool m_Dirty;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif