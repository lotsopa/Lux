#ifndef LUX_PHYSICS_MATERIAL_H
#define LUX_PHYSICS_MATERIAL_H
#include "LuxComponent.h"

namespace Lux
{
	namespace Physics
	{
		class PhysicsMaterial : public Core::Component
		{
		public:
			PhysicsMaterial();
			virtual ~PhysicsMaterial();

			inline void SetRestitution(float a_Val) { m_Properties->setRestitution(a_Val); }
			inline float GetRestitution() { return m_Properties->getRestitution(); }
			inline void SetDynamicFriction(float a_Val) { m_Properties->setDynamicFriction(a_Val); }
			inline float GetDynamicFriction() { return m_Properties->getDynamicFriction(); }
			inline void SetStaticFriction(float a_Val) { m_Properties->setStaticFriction(a_Val); }
			inline float GetStaticFriction() { return m_Properties->getStaticFriction(); }

		private:
			PxMaterial* m_Properties;
			friend class PhysicsSystem;

		protected:
			void Reset();
			friend class Core::ComponentFactory;
		};
	}
}

#endif