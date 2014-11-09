#include "LuxPCH.h"
#include "LuxComponent.h"
#include "LuxLight.h"

Lux::Graphics::Light::Light() : m_Color(1.0f), m_Type(DIRECTIONAL_LIGHT)
{

}

Lux::Graphics::Light::~Light()
{

}

void Lux::Graphics::Light::Reset()
{
	m_Color = vec4(1.0f);
	m_Type = DIRECTIONAL_LIGHT;
}
