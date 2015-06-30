#ifndef LUX_COLLIDER_H
#define LUX_COLLIDER_H

#include "LuxComponent.h"

namespace Lux
{
	namespace Physics
	{
		class Collider : public Core::Component
		{
		public:
			Collider();
			virtual ~Collider();

		protected:
			virtual void Reset() = 0;
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
			PxShape* m_Shape;
		};
	}
}

#endif