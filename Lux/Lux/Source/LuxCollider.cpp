#include "LuxPCH.h"
#include "LuxCollider.h"

Lux::Physics::Collider::Collider() : Component(), m_Shape(nullptr), m_IsTrigger(false)
{

}

Lux::Physics::Collider::~Collider()
{
	Reset();
}

void Lux::Physics::Collider::Reset()
{
	if (m_Shape)
	{
		m_Shape->release();
		m_Shape = nullptr;
	}
	m_IsTrigger = false;
}

void Lux::Physics::Collider::SetIsTrigger(bool a_Trigger)
{
	m_IsTrigger = a_Trigger;

	if (m_Shape)
	{
		m_Shape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !m_IsTrigger);
		m_Shape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, m_IsTrigger);
	}
}

