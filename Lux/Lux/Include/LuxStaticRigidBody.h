#ifndef LUX_STATIC_RIGID_BODY_H
#define LUX_STATIC_RIGID_BODY_H

#include "LuxComponent.h"

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
			virtual ~StaticRigidBody();

			inline Core::PhysicsMaterial* GetPhysicsMaterial() { return m_Material; }
			inline void SetPhysicsMaterial(Core::PhysicsMaterial* a_Material) { m_Material = a_Material; m_Dirty = true; }
		private:
			PxRigidStatic* m_Properties;
			Core::PhysicsMaterial* m_Material;
			bool m_Dirty;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif