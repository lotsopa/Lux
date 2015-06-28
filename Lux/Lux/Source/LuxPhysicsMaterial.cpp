#include "LuxPCH.h"
#include "LuxPhysicsMaterial.h"

Lux::Core::PhysicsMaterial::PhysicsMaterial() : 
m_Properties(nullptr), 
m_DynamicFriction(0.0f),
m_StaticFriction(0.0f), 
m_Restitution(0.0f)
{

}

Lux::Core::PhysicsMaterial::PhysicsMaterial(float a_Restitution, float a_DynamicFriction, float a_StaticFriction) : 
m_Properties(nullptr), 
m_Restitution(a_Restitution),
m_DynamicFriction(a_DynamicFriction),
m_StaticFriction(a_StaticFriction)
{

}

Lux::Core::PhysicsMaterial::~PhysicsMaterial()
{
	if (m_Properties)
	{
		m_Properties->release();
		m_Properties = nullptr;
	}
}
