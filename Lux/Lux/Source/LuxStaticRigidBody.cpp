#include "LuxPCH.h"
#include "LuxStaticRigidBody.h"

Lux::Physics::StaticRigidBody::StaticRigidBody() : Component(), m_Properties(nullptr), m_Dirty(false)
{

}

Lux::Physics::StaticRigidBody::StaticRigidBody(const StaticRigidBody& a_Other)
{
	m_Properties = a_Other.m_Properties;
	m_Index = a_Other.m_Index;
	m_Material.reset(a_Other.m_Material.get());
	m_Dirty = a_Other.m_Dirty;
}

Lux::Physics::StaticRigidBody::~StaticRigidBody()
{

}

void Lux::Physics::StaticRigidBody::Reset()
{
	if (m_Properties)
	{
		m_Properties->release();
		m_Properties = nullptr;
	}
	m_Material.release();
	m_Dirty = false;
}
