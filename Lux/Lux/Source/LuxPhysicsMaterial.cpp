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

float Lux::Core::PhysicsMaterial::GetStaticFriction()
{
	return m_StaticFriction;
}

void Lux::Core::PhysicsMaterial::SetStaticFriction(float a_Val)
{
	m_StaticFriction = a_Val;

	if (m_Properties)
	{
		m_Properties->setStaticFriction(m_StaticFriction);
	}
}

float Lux::Core::PhysicsMaterial::GetDynamicFriction()
{
	return m_DynamicFriction;
}

void Lux::Core::PhysicsMaterial::SetDynamicFriction(float a_Val)
{
	m_DynamicFriction = a_Val;

	if (m_Properties)
	{
		m_Properties->setDynamicFriction(m_DynamicFriction);
	}
}

float Lux::Core::PhysicsMaterial::GetRestitution()
{
	return m_Restitution;
}

void Lux::Core::PhysicsMaterial::SetRestitution(float a_Val)
{
	m_Restitution = a_Val;

	if (m_Properties)
	{
		m_Properties->setRestitution(m_Restitution);
	}
}
