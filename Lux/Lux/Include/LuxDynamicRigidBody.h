#ifndef LUX_DYNAMIC_RIGID_BODY_H
#define LUX_DYNAMIC_RIGID_BODY_H
#include "LuxComponent.h"

namespace Lux
{
	namespace Physics
	{
		class DynamicRigidBody : public Core::Component
		{
		public:
			DynamicRigidBody();
			virtual ~DynamicRigidBody();

		private:
			PxRigidDynamic* m_Properties;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif