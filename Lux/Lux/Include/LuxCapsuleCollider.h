#ifndef LUX_CAPSULE_COLLIDER_H
#define LUX_CAPSULE_COLLIDER_H
#include "LuxCollider.h"

namespace Lux
{
	namespace Physics
	{
		class CapsuleCollider : public Collider
		{
		public:
			CapsuleCollider();
			CapsuleCollider(const CapsuleCollider& a_Other);
			virtual ~CapsuleCollider();

			enum CapsuleOrientation
			{
				X_AXIS = 0,
				Y_AXIS,
				Z_AXIS
			};

			inline float GetRadius() { return m_Radius; }
			void SetRadius(float a_Radius);

			inline float GetHalfHeight() { return m_HalfHeight; }
			void SetHalfHeight(float a_HalfHeight);

			inline CapsuleOrientation GetOrientation() { return m_Orientation; }
			void SetOrientation(CapsuleOrientation a_Orientation);

		private:
			float m_Radius;
			float m_HalfHeight;
			CapsuleOrientation m_Orientation;

		protected:
			virtual void Reset();
			friend class Core::ComponentFactory;
			friend class PhysicsSystem;
		};
	}
}

#endif