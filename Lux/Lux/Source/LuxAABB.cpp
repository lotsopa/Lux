#include "LuxPCH.h"
#include "LuxAABB.h"

Lux::Core::AABB::AABB()
{

}

Lux::Core::AABB::AABB(const vec3& a_Min, const vec3& a_Max) :
m_Min(a_Min), m_Max(a_Max)
{
	vec3 size = m_Max - m_Min;
	m_HalfExtents = size / 2.0f;
}

Lux::Core::AABB::~AABB()
{

}

void Lux::Core::AABB::SetMax(const vec3& a_Max)
{
	m_Max = a_Max;
	vec3 size = m_Max - m_Min;
	m_HalfExtents = size / 2.0f;
}

void Lux::Core::AABB::SetMin(const vec3& a_Min)
{
	m_Min = a_Min;
	vec3 size = m_Max - m_Min;
	m_HalfExtents = size / 2.0f;
}
