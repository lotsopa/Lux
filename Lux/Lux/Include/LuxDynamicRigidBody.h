#ifndef LUX_DYNAMIC_RIGID_BODY_H
#define LUX_DYNAMIC_RIGID_BODY_H
#include "LuxComponent.h"

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
			virtual ~DynamicRigidBody();

			inline Core::PhysicsMaterial* GetPhysicsMaterial() { return m_Material; }
			inline void SetPhysicsMaterial(Core::PhysicsMaterial* a_Material) { m_Material = a_Material; }

		private:
			PxRigidDynamic* m_Properties;
			Core::PhysicsMaterial* m_Material;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif