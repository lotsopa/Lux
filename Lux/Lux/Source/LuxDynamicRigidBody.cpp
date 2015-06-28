#include "LuxPCH.h"
#include "LuxDynamicRigidBody.h"

Lux::Physics::DynamicRigidBody::DynamicRigidBody() : Component(), m_Properties(nullptr), m_Material(nullptr), m_Dirty(false)
{

}

Lux::Physics::DynamicRigidBody::~DynamicRigidBody()
{

}

void Lux::Physics::DynamicRigidBody::Reset()
{
	if (m_Properties)
	{
		m_Properties->release();
		m_Properties = nullptr;
	}
	m_Material = nullptr;
	m_Dirty = false;
}
