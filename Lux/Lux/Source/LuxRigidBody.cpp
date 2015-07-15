#include "LuxPCH.h"
#include "LuxRigidBody.h"

Lux::Physics::RigidBody::RigidBody() : Component(), m_Properties(nullptr)
{
	m_Type = RigidBodyType::RIGID_BODY_STATIC;
}

Lux::Physics::RigidBody::~RigidBody()
{

}

void Lux::Physics::RigidBody::Reset()
{
	if (m_Properties)
	{
		m_Properties->release();
		m_Properties = nullptr;
	}
	m_Material.release();
	m_Type = RIGID_BODY_STATIC;
}

void Lux::Physics::RigidBody::SetPhysicsMaterial(Core::ObserverPtr<Core::PhysicsMaterial>& a_Material)
{
	m_Material.reset(a_Material.get());
	m_MessageManager->BroadcastMessage(MSG_PHYS_MAT_SET, a_Material.get());
}

