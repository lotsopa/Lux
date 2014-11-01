#include "LuxPCH.h"
#include "LuxTransform.h"

Lux::Core::Transform::Transform() :
Component(),
m_TransformMatrix()
{

}

Lux::Core::Transform::Transform(const mat4x4& a_TMat) :
m_TransformMatrix(a_TMat)
{

}

Lux::Core::Transform::~Transform()
{

}

void Lux::Core::Transform::Reset()
{
	m_TransformMatrix = mat4x4();
}

void Lux::Core::Transform::Translate(const vec3& a_Amount)
{
	m_TransformMatrix = translate(m_TransformMatrix, a_Amount);
}
