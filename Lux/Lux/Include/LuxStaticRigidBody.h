#ifndef LUX_STATIC_RIGID_BODY_H
#define LUX_STATIC_RIGID_BODY_H

#include "LuxRigidBody.h"

namespace Lux
{
	namespace Core
	{
		class PhysicsMaterial;
	}

	namespace Physics
	{
		class StaticRigidBody : public RigidBody
		{
		public:
			StaticRigidBody();
			StaticRigidBody(const StaticRigidBody& a_Other);
			virtual ~StaticRigidBody();

			inline void SetPhysicsMaterial(Core::ObserverPtr<Core::PhysicsMaterial>& a_Material)
			{
				m_Material.reset(a_Material.get());
			}

			inline Core::ObserverPtr<Core::PhysicsMaterial>& GetPhysicsMaterial()
			{
				return m_Material;
			}

		private:
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif