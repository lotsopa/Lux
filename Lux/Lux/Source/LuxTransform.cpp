#include "LuxPCH.h"
#include "LuxTransform.h"

Lux::Core::Transform::Transform() :
Component(),
m_TransformMatrix(),
m_Scale(1.0f),
m_Position(0.0f),
m_Rotation(),
m_LocalRotation(),
m_TransformDirty(true)
{

}

Lux::Core::Transform::~Transform()
{

}

void Lux::Core::Transform::Reset()
{
	m_TransformMatrix = mat4x4();
	m_Position = vec3(0.0f);
	m_Scale = vec3(1.0f);
	m_Rotation = quat();
	m_LocalRotation = quat();
	m_TransformDirty = true;
}

void Lux::Core::Transform::ApplyTransform()
{
	if (m_TransformDirty)
	{
		m_TransformMatrix = mat4x4(1.0f);
		m_TransformMatrix *= toMat4(m_LocalRotation);
		m_TransformMatrix = translate(m_TransformMatrix, m_Position);
		m_TransformMatrix *= toMat4(m_Rotation);
		m_TransformMatrix = scale(m_TransformMatrix, m_Scale);

		// Calculate forward vector
		m_Forward =  vec3(0, 0, -1) * m_LocalRotation;

		// Calculate Right vector
		m_Right = vec3(1, 0, 0) * m_LocalRotation;

		// Calculate Up vector
		m_Up = vec3(0, 1, 0) * m_LocalRotation;

		m_InverseTranslationMatrix = m_TransformMatrix;
		m_InverseTranslationMatrix[3][0] *= -1;
		m_InverseTranslationMatrix[3][1] *= -1;
		m_InverseTranslationMatrix[3][2] *= -1;

		m_TransformDirty = false;
	}
}

void Lux::Core::Transform::SetPosition(const vec3& a_Pos)
{
	m_Position = a_Pos;
	m_TransformDirty = true;
}

Lux::vec3& Lux::Core::Transform::GetPosition()
{
	return m_Position;
}

void Lux::Core::Transform::Translate(const vec3& a_Amount)
{
	m_Position += a_Amount;
	m_TransformDirty = true;
}

void Lux::Core::Transform::Rotate(const float a_Angle, const vec3& a_Axis, SpaceRelativeTo a_Space)
{
	switch (a_Space)
	{
	case SELF:
		m_LocalRotation = angleAxis(radians(a_Angle), normalize(a_Axis)) * m_LocalRotation;
		break;

	case WORLD:
		m_Rotation = angleAxis(radians(a_Angle), normalize(a_Axis)) * m_Rotation;
		break;
	}
	
	m_TransformDirty = true;
}

void Lux::Core::Transform::Scale(const vec3& a_Amount)
{
	m_Scale += a_Amount;
	m_TransformDirty = true;
}

void Lux::Core::Transform::SetScale(const vec3& a_Scale)
{
	m_Scale = a_Scale;
	m_TransformDirty = true;
}

void Lux::Core::Transform::SetRotation(const quat& a_Rot)
{
	m_Rotation = a_Rot;
	m_TransformDirty = true;
}

const Lux::vec3& Lux::Core::Transform::GetScale()
{
	return m_Scale;
}

const Lux::quat& Lux::Core::Transform::GetRotation()
{
	return m_Rotation;
}

const Lux::vec3& Lux::Core::Transform::GetForwardVector() const
{
	return m_Forward;
}

const Lux::vec3& Lux::Core::Transform::GetUpVector() const
{
	return m_Up;
}

const Lux::vec3& Lux::Core::Transform::GetRightVector() const
{
	return m_Right;
}

const Lux::quat& Lux::Core::Transform::GetLocalRotation() const
{
	return m_LocalRotation;
}

void Lux::Core::Transform::SetLocalRotation(const quat& a_Rot)
{
	m_LocalRotation = a_Rot;
	m_TransformDirty = true;
}
