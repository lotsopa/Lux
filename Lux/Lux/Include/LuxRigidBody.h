#ifndef LUX_RIGID_BODY_H
#define LUX_RIGID_BODY_H

#include "LuxComponent.h"
#include "LuxObserverPtr.h"
#include "LuxPhysicsMaterial.h"

namespace Lux
{
	namespace Physics
	{
		class RigidBody : public Core::Component
		{
		public:
			RigidBody();
			virtual ~RigidBody();


			enum RigidBodyType
			{
				RIGID_BODY_STATIC = 0,
				RIGID_BODY_DYNAMIC
			};

			inline const RigidBodyType GetType() { return m_Type; }

			void SetPhysicsMaterial(Core::ObserverPtr<Core::PhysicsMaterial>& a_Material);

			inline Core::ObserverPtr<Core::PhysicsMaterial>& GetPhysicsMaterial()
			{
				return m_Material;
			}

		protected:
			virtual void Reset() = 0;
			RigidBodyType m_Type;
			PxRigidActor* m_Properties;
			Core::ObserverPtr<Core::PhysicsMaterial> m_Material;
			friend class PhysicsSystem;
			friend class Core::ComponentFactory;
		};
	}
}

#endif