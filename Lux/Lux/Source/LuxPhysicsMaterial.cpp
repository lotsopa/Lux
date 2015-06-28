#include "LuxPCH.h"
#include "LuxPhysicsMaterial.h"

Lux::Physics::PhysicsMaterial::PhysicsMaterial() : Component(), m_Properties(nullptr)
{

}

Lux::Physics::PhysicsMaterial::~PhysicsMaterial()
{
	Reset();
}

void Lux::Physics::PhysicsMaterial::Reset()
{
	if (m_Properties)
	{
		m_Properties->release();
		m_Properties = nullptr;
	}
}
