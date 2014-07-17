#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxDefaultComponents.h"

Lux::Transform::Transform() :
m_TransformMatrix()
{

}

Lux::Transform::Transform(mat4x4& a_TMatrix) : 
m_TransformMatrix(a_TMatrix)
{

}

Lux::Transform::~Transform()
{

}
