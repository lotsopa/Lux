#include "LuxPCH.h"
#include "LuxCamera.h"

Lux::Graphics::Camera::Camera() : Component()
{
	Reset();
}

Lux::Graphics::Camera::~Camera()
{

}

void Lux::Graphics::Camera::SetProjectionPersp(const float a_FovY, const float a_Aspect, const float a_ZNear, const float a_ZFar)
{
	m_ProjectionMatrix = perspective(radians(a_FovY), a_Aspect, a_ZNear, a_ZFar);
}

void Lux::Graphics::Camera::SetProjectionOrtho(const float a_Right, const float a_Left, const float a_Top, const float a_Bottom)
{
	m_ProjectionMatrix = ortho(a_Left, a_Right, a_Bottom, a_Top);
}

void Lux::Graphics::Camera::Reset()
{
	m_ProjectionMatrix = mat4x4(1.0f);
	m_IsMainCamera = false;
}
