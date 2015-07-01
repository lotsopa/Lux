#include "LuxPCH.h"
#include "LuxDynamicRigidBody.h"

Lux::Physics::DynamicRigidBody::DynamicRigidBody() : RigidBody()
{
	m_Type = RigidBodyType::RIGID_BODY_DYNAMIC;
}

Lux::Physics::DynamicRigidBody::DynamicRigidBody(const DynamicRigidBody& a_Other)
{
	m_Properties = a_Other.m_Properties;
	m_Index = a_Other.m_Index;
	m_Material.reset(a_Other.m_Material.get());
	m_Type = a_Other.m_Type;
}

Lux::Physics::DynamicRigidBody::~DynamicRigidBody()
{

}

void Lux::Physics::DynamicRigidBody::Reset()
{
	RigidBody::Reset();
	m_Type = RigidBody::RIGID_BODY_DYNAMIC;
}
