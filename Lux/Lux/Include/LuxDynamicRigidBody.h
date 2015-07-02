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

			void SetIsKinematic(bool a_Kinematic);
			inline bool GetIsKinematic() { return m_IsKinematic; }
		private:
			bool m_IsKinematic;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif