#ifndef LUX_SPHERE_COLLIDER_H
#define LUX_SPHERE_COLLIDER_H
#include "LuxCollider.h"

namespace Lux
{
	namespace Physics
	{
		class SphereCollider : public Collider
		{
		public:
			SphereCollider();
			SphereCollider(const SphereCollider& a_Other);
			virtual ~SphereCollider();

			inline float GetRadius() { return m_Radius; }
			void SetRadius(float a_Radius);
		private:
			float m_Radius;

		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
		};
	}
}

#endif