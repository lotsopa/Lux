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
	m_ProjectionMatrix = perspective(a_FovY, a_Aspect, a_ZNear, a_ZFar);
}

void Lux::Graphics::Camera::SetProjectionOrtho(const float a_Right, const float a_Left, const float a_Top, const float a_Bottom)
{
	m_ProjectionMatrix = ortho(a_Left, a_Right, a_Bottom, a_Top);
}

void Lux::Graphics::Camera::Reset()
{
	m_ProjectionMatrix = mat4x4(1.0f);
	m_ViewMatrix = mat4x4(1.0f);
	m_Scale = vec3(1.0f);
	m_Rotation = quat();
	m_Translation = vec3(0.0f);
	m_ViewDirty = true;
	m_IsMainCamera = false;
}

void Lux::Graphics::Camera::ApplyViewTransform()
{
	if (m_ViewDirty)
	{
		m_ViewMatrix = mat4(1.0f);
		m_ViewMatrix = translate(m_ViewMatrix, -m_Translation);
		m_ViewMatrix *= toMat4(m_Rotation);
		m_ViewMatrix = scale(m_ViewMatrix, m_Scale);
		m_ViewDirty = false;
	}
}

void Lux::Graphics::Camera::SetPosition(const vec3& a_Pos)
{
	m_Translation = a_Pos;
	m_ViewDirty = true;
}

const Lux::vec3& Lux::Graphics::Camera::GetPosition()
{
	return m_Translation;
}

void Lux::Graphics::Camera::Translate(const vec3& a_Amount)
{
	m_Translation += a_Amount;
	m_ViewDirty = true;
}

void Lux::Graphics::Camera::Rotate(const float a_Angle, const vec3& a_Axis)
{
	m_Rotation = m_Rotation * glm::angleAxis(a_Angle, a_Axis);
	m_ViewDirty = true;
}

void Lux::Graphics::Camera::Scale(const vec3& a_Amount)
{
	m_Scale += a_Amount;
	m_ViewDirty = true;
}

void Lux::Graphics::Camera::SetScale(const vec3& a_Scale)
{
	m_Scale = a_Scale;
	m_ViewDirty = true;
}

void Lux::Graphics::Camera::SetRotation(const quat& a_Rot)
{
	m_Rotation = a_Rot;
	m_ViewDirty = true;
}

const Lux::vec3& Lux::Graphics::Camera::GetScale()
{
	return m_Scale;
}

const Lux::quat& Lux::Graphics::Camera::GetRotation()
{
	return m_Rotation;
}
