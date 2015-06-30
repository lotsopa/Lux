#ifndef LUX_BOX_COLLIDER_H
#define LUX_BOX_COLLIDER_H

#include "LuxCollider.h"

namespace Lux
{
	namespace Physics
	{
		class BoxCollider : public Collider
		{
		public:
			BoxCollider();
			BoxCollider(const BoxCollider& a_Other);
			virtual ~BoxCollider();

			void SetHalfExtents(vec3& a_HlfExtents);
			const vec3 GetHalfExtents() const;
		private:
			vec3 m_HalfExtents;
		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
		};
	}
}

#endif