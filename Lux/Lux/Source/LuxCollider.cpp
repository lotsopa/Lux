#include "LuxPCH.h"
#include "LuxCollider.h"

Lux::Physics::Collider::Collider() : Component(), m_Shape(nullptr)
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
}

