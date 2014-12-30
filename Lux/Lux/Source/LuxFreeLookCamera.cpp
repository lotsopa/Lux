#include "LuxPCH.h"
#include "LuxObjectPool.h"
#include "LuxKeyCodes.h"
#include "LuxComponent.h"
#include "LuxTransform.h"
#include "LuxEventListener.h"
#include "LuxKey.h"
#include "LuxSceneManager.h"
#include "LuxBehaviourComponent.h"
#include "LuxFreeLookCamera.h"

Lux::Core::FreeLookCamera::FreeLookCamera() : BehaviourComponent(), m_RotationMultiplier(0.1f), m_SpeedMultiplier(2.0f)
{

}

Lux::Core::FreeLookCamera::~FreeLookCamera()
{

}

void Lux::Core::FreeLookCamera::Reset()
{

}

void Lux::Core::FreeLookCamera::Start()
{
	m_LastMousePos = m_EventListener->GetCursorPos();
	m_Transform = GetComponent<Transform>();
}

void Lux::Core::FreeLookCamera::Update(const float a_DeltaTime)
{
	vec2 currMousePos = m_EventListener->GetCursorPos();

	if (m_EventListener->GetMouseButtonDown(LUX_MOUSE_BUTTON_RIGHT))
	{
		vec2 mouseDelta = currMousePos - m_LastMousePos;
		m_EulerRotation.x = clamp<float>(m_EulerRotation.x + mouseDelta.y * m_RotationMultiplier * a_DeltaTime, radians(-80.0f), radians(80.0f));
		m_EulerRotation.y += mouseDelta.x * m_RotationMultiplier * a_DeltaTime;
		m_EulerRotation.z = 0.0f;
		m_Transform->GetRawPtr()->SetLocalRotation(angleAxis(m_EulerRotation.x, vec3(1, 0, 0)) * angleAxis(m_EulerRotation.y, vec3(0, 1, 0)) * angleAxis(m_EulerRotation.z, vec3(0, 0, 1)));
	}

	if (m_EventListener->GetKeyDown(KeyCode::LUX_KEY_W))
	{
		m_Transform->GetRawPtr()->Translate(m_Transform->GetRawPtr()->GetForwardVector() * m_SpeedMultiplier * a_DeltaTime);
	}

	if (m_EventListener->GetKeyDown(KeyCode::LUX_KEY_S))
	{
		m_Transform->GetRawPtr()->Translate(-m_Transform->GetRawPtr()->GetForwardVector() * m_SpeedMultiplier * a_DeltaTime);
	}

	if (m_EventListener->GetKeyDown(KeyCode::LUX_KEY_A))
	{
		m_Transform->GetRawPtr()->Translate(-m_Transform->GetRawPtr()->GetRightVector() * m_SpeedMultiplier * a_DeltaTime);
	}

	if (m_EventListener->GetKeyDown(KeyCode::LUX_KEY_D))
	{
		m_Transform->GetRawPtr()->Translate(m_Transform->GetRawPtr()->GetRightVector() * m_SpeedMultiplier * a_DeltaTime);
	}

	m_LastMousePos = currMousePos;
}

void Lux::Core::FreeLookCamera::OnDestroy()
{

}
