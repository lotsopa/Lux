#include "LuxPCH.h"
#include "LuxStaticRigidBody.h"

Lux::Physics::StaticRigidBody::StaticRigidBody() : Component(), m_Properties(nullptr)
{

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
}
