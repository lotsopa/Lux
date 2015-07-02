#include "LuxPCH.h"
#include "LuxSphereCollider.h"

Lux::Physics::SphereCollider::SphereCollider() : m_Radius(1.0f)
{

}

Lux::Physics::SphereCollider::SphereCollider(const SphereCollider& a_Other)
{
	m_Index = a_Other.m_Index;
	m_Radius = a_Other.m_Radius;
	m_Shape = a_Other.m_Shape;
}

Lux::Physics::SphereCollider::~SphereCollider()
{

}

void Lux::Physics::SphereCollider::Reset()
{
	Collider::Reset();
	m_Radius = 1.0f;
}

void Lux::Physics::SphereCollider::SetRadius(float a_Radius)
{
	m_Radius = a_Radius;
	if (m_Shape)
	{
		m_Shape->setGeometry(PxSphereGeometry(a_Radius));
	}
}