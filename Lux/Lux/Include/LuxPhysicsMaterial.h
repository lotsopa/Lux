#ifndef LUX_PHYSICS_MATERIAL_H
#define LUX_PHYSICS_MATERIAL_H

namespace Lux
{
	namespace Physics
	{
		class PhysicsSystem;
	}

	namespace Core
	{
		class PhysicsMaterial
		{
		public:
			PhysicsMaterial();
			PhysicsMaterial(float a_Restitution, float a_DynamicFriction, float a_StaticFriction);
			virtual ~PhysicsMaterial();

			void SetRestitution(float a_Val);
			float GetRestitution();
			void SetDynamicFriction(float a_Val);
			float GetDynamicFriction();
			void SetStaticFriction(float a_Val);
			float GetStaticFriction();

		private:
			float m_Restitution;
			float m_DynamicFriction;
			float m_StaticFriction;
			PxMaterial* m_Properties;
			friend class Physics::PhysicsSystem;
		};
	}
}

#endif