#ifndef LUX_PHYSICS_MATERIAL_H
#define LUX_PHYSICS_MATERIAL_H

namespace Lux
{
	namespace Core
	{
		class PhysicsMaterial
		{
		public:
			PhysicsMaterial();
			PhysicsMaterial(float a_Restitution, float a_DynamicFriction, float a_StaticFriction);
			virtual ~PhysicsMaterial();

			inline void SetRestitution(float a_Val) { m_Restitution = a_Val; }
			inline float GetRestitution() { return m_Restitution; }
			inline void SetDynamicFriction(float a_Val) { m_DynamicFriction = a_Val; }
			inline float GetDynamicFriction() { return m_DynamicFriction; }
			inline void SetStaticFriction(float a_Val) { m_StaticFriction = a_Val; }
			inline float GetStaticFriction() { return m_StaticFriction; }

		private:
			float m_Restitution;
			float m_DynamicFriction;
			float m_StaticFriction;
			PxMaterial* m_Properties;
		};
	}
}

#endif