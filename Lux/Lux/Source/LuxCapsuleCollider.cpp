#include "LuxPCH.h"
#include "LuxCapsuleCollider.h"

Lux::Physics::CapsuleCollider::CapsuleCollider() : Collider(), m_HalfHeight(1.0f), m_Radius(1.0f), m_Orientation(X_AXIS)
{

}

Lux::Physics::CapsuleCollider::CapsuleCollider(const CapsuleCollider& a_Other)
{
	m_HalfHeight = a_Other.m_HalfHeight;
	m_Radius = a_Other.m_Radius;
	m_IsTrigger = a_Other.m_IsTrigger;
	m_Index = a_Other.m_Index;
	m_Shape = a_Other.m_Shape;
	m_Orientation = a_Other.m_Orientation;
}

Lux::Physics::CapsuleCollider::~CapsuleCollider()
{

}

void Lux::Physics::CapsuleCollider::SetRadius(float a_Radius)
{
	LuxAssert(a_Radius > 0.0f);
	m_Radius = a_Radius;
	if (m_Shape)
	{
		m_Shape->setGeometry(PxCapsuleGeometry(m_Radius, m_HalfHeight));
	}
}

void Lux::Physics::CapsuleCollider::SetHalfHeight(float a_HalfHeight)
{
	LuxAssert(a_HalfHeight > 0.0f);
	m_HalfHeight = a_HalfHeight;
	if (m_Shape)
	{
		m_Shape->setGeometry(PxCapsuleGeometry(m_Radius, m_HalfHeight));
	}
}

void Lux::Physics::CapsuleCollider::Reset()
{
	Collider::Reset();
	m_Radius = 1.0f;
	m_HalfHeight = 1.0f;
	m_Orientation = X_AXIS;
}

void Lux::Physics::CapsuleCollider::SetOrientation(CapsuleOrientation a_Orientation)
{
	m_Orientation = a_Orientation;

	if (m_Shape)
	{
		PxTransform relativePose;
		switch (m_Orientation)
		{
		case X_AXIS:
			relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(0, 1, 0)));
			break;

		case Y_AXIS:
			relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
			break;

		case Z_AXIS:
			relativePose = PxTransform(PxQuat(PxHalfPi, PxVec3(1, 0, 0)));
			break;

		default:
			Utility::ThrowError("Error Setting Capsule Orientation. Unsupported Enumerator.");
			break;
		}
		m_Shape->setLocalPose(relativePose);
	}
}

