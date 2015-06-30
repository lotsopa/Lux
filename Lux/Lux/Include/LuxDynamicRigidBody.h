#ifndef LUX_DYNAMIC_RIGID_BODY_H
#define LUX_DYNAMIC_RIGID_BODY_H
#include "LuxRigidBody.h"

namespace Lux
{
	namespace Core
	{
		class PhysicsMaterial;
	}

	namespace Physics
	{
		class DynamicRigidBody : public RigidBody
		{
		public:
			DynamicRigidBody();
			DynamicRigidBody(const DynamicRigidBody& a_Other);
			virtual ~DynamicRigidBody();

		private:
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif