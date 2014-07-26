#include "LuxPCH.h"
#include "LuxTransform.h"

Lux::Transform::Transform() :
Component(),
m_TransformMatrix()
{

}

Lux::Transform::Transform(const mat4x4& a_TMat) :
m_TransformMatrix(a_TMat)
{

}

Lux::Transform::~Transform()
{

}

void Lux::Transform::Reset()
{
	m_TransformMatrix = mat4x4();
}
