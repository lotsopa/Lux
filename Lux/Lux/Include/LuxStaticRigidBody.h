#ifndef LUX_STATIC_RIGID_BODY_H
#define LUX_STATIC_RIGID_BODY_H

#include "LuxComponent.h"

namespace Lux
{
	namespace Physics
	{
		class StaticRigidBody : public Core::Component
		{
		public:
			StaticRigidBody();
			virtual ~StaticRigidBody();

		private:
			PxRigidStatic* m_Properties;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif