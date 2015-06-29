#include "LuxPCH.h"
#include "LuxDynamicRigidBody.h"

Lux::Physics::DynamicRigidBody::DynamicRigidBody() : Component(), m_Properties(nullptr), m_Dirty(false)
{

}

Lux::Physics::DynamicRigidBody::DynamicRigidBody(const DynamicRigidBody& a_Other)
{
	m_Properties = a_Other.m_Properties;
	m_Index = a_Other.m_Index;
	m_Material.reset(a_Other.m_Material.get());
	m_Dirty = a_Other.m_Dirty;
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
	m_Material.release();
	m_Dirty = false;
}
