#include "LuxPCH.h"
#include "LuxStaticRigidBody.h"

Lux::Physics::StaticRigidBody::StaticRigidBody() : RigidBody()
{
	m_Type = RigidBodyType::RIGID_BODY_STATIC;
}

Lux::Physics::StaticRigidBody::StaticRigidBody(const StaticRigidBody& a_Other)
{
	m_Properties = a_Other.m_Properties;
	m_Material.reset(a_Other.m_Material.get());
	m_Type = a_Other.m_Type;
}

Lux::Physics::StaticRigidBody::~StaticRigidBody()
{

}

void Lux::Physics::StaticRigidBody::Reset()
{
	RigidBody::Reset();
	m_Type = RigidBody::RIGID_BODY_STATIC;
}
