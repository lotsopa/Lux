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
	m_ZNear = a_ZNear;
	m_ZFar = a_ZFar;
	m_FovY = a_FovY;
	m_Aspect = a_Aspect;
	m_ProjectionMatrix = perspective(radians(a_FovY), a_Aspect, a_ZNear, a_ZFar);
}

void Lux::Graphics::Camera::SetProjectionOrtho(const float a_Right, const float a_Left, const float a_Top, const float a_Bottom, const float a_ZNear, const float a_ZFar)
{
	m_ZNear = a_ZNear;
	m_ZFar = a_ZFar;
	m_Left = a_Left;
	m_Right = a_Right;
	m_Top = a_Top;
	m_Bottom = a_Bottom;
	m_ProjectionMatrix = ortho(a_Left, a_Right, a_Bottom, a_Top, a_ZNear, a_ZFar);
}

void Lux::Graphics::Camera::Reset()
{
	m_ProjectionMatrix = mat4x4(1.0f);
	m_IsMainCamera = false;
	m_ZFar = 1000.0f;
	m_ZNear = 0.1f;
	m_Aspect = 16.0f / 9.0f;
	m_FovY = 60.0f;
	m_Left = -1.0f;
	m_Right = 1.0f;
	m_Top = 1.0f;
	m_Bottom = -1.0f;
}

void Lux::Graphics::Camera::ChangeAspect(const float a_Aspect)
{
	m_Aspect = a_Aspect;
	m_ProjectionMatrix = perspective(radians(m_FovY), m_Aspect, m_ZNear, m_ZFar);
}
