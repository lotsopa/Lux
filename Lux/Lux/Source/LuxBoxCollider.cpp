#include "LuxPCH.h"
#include "LuxBoxCollider.h"

Lux::Physics::BoxCollider::BoxCollider() : Collider(), m_HalfExtents(1.0f, 1.0f, 1.0f)
{

}

Lux::Physics::BoxCollider::BoxCollider(const BoxCollider& a_Other)
{
	m_HalfExtents = a_Other.m_HalfExtents;
	m_Shape = a_Other.m_Shape;
	m_Index = a_Other.m_Index;
}

Lux::Physics::BoxCollider::~BoxCollider()
{

}

void Lux::Physics::BoxCollider::Reset()
{
	Collider::Reset();
	m_HalfExtents = vec3();
}

const Lux::vec3 Lux::Physics::BoxCollider::GetHalfExtents() const
{
	return m_HalfExtents;
}

void Lux::Physics::BoxCollider::SetHalfExtents(vec3& a_HlfExtents)
{
	m_HalfExtents = a_HlfExtents;

	if (m_Shape)
	{
		PxBoxGeometry* box = (PxBoxGeometry*)m_Shape;
		box->halfExtents = (Utility::ConvertVec3ToPhysX(m_HalfExtents));
	}
}
